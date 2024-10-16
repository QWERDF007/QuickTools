#include "imgproc/ImageHistogram.h"

#include "common/Utils.h"
#include "core/Error.h"
#include "core/QuickToolSettings.h"
#include "core/RuntimeParams.h"

#include <opencv2/opencv.hpp>

#include <QFile>
#include <QString>
#include <QtQml>
#include <chrono>
#include <vector>

namespace quicktools::imgproc {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

namespace {

QString getMarkdown()
{
    QFile markdown_file("docs/imgproc/ImageHistogram.md");
    if (!markdown_file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";
    return markdown_file.readAll();
}

std::tuple<int, std::tuple<float, float>> getHistSizeAndRange(const QString &color_space, const int ch)
{
    if (color_space == "HSV" && ch == 0)
        return std::make_tuple(180, std::make_tuple(0.f, 180.f));
    else
        return std::make_tuple(256, std::make_tuple(0.f, 256.f));
}

class ImageHistogramRuntimeParams : public core::RuntimeParams
{
public:
    ImageHistogramRuntimeParams(QObject *parent = nullptr)
        : core::RuntimeParams(parent)
    {
        reader_.setNameFilters(common::FileReader::ImageFilters);
    }

    void reset() override;

    bool getInput(core::InputParams *input_params) override;
    bool genOutput(core::OutputParams *output_params) override;

    struct RuntimeInput
    {
        QString image_path;
        QString color_space;
    };

    struct RuntimeOutput
    {
        int                    chs;
        QList<QList<QVariant>> hists_data;
        QList<QList<QVariant>> hists_min;
        QList<QList<QVariant>> hists_max;
    };

    RuntimeInput  input;
    RuntimeOutput output;

private:
    common::FileReader reader_;

    friend class ::quicktools::imgproc::ImageHistogram;
};

bool ImageHistogramRuntimeParams::getInput(core::InputParams *input_params)
{
    if (input_params == nullptr)
    {
        setError(Error::InputParamsEmpty);
        return false;
    }
    const QString root = input_params->data("Image", QuickToolParamRole::ParamValueRole).toString();
    input.image_path   = reader_.read(root, true, true);
    if (input.image_path.isEmpty())
    {
        setError(Error::ImageFilePathEmpty);
        return false;
    }
    if (!QFile::exists(input.image_path))
    {
        setError(Error::FileNotFound, input.image_path);
        return false;
    }
    input.color_space = input_params->data("ColorSpace", QuickToolParamRole::ParamValueRole).toString();
    return true;
}

bool ImageHistogramRuntimeParams::genOutput(core::OutputParams *output_params)
{
    if (output_params == nullptr)
    {
        setError(Error::OutputParamsEmpty);
        return false;
    }
    output_params->setData("ImagePath", input.image_path);
    output_params->setData("Channels", output.chs);
    output_params->setData("Hist", QVariant::fromValue(output.hists_data));
    output_params->setData("HistMin", QVariant::fromValue(output.hists_min));
    output_params->setData("HistMax", QVariant::fromValue(output.hists_max));
    return true;
}

void ImageHistogramRuntimeParams::reset()
{
    output.hists_data.clear();
    output.hists_max.clear();
    output.hists_min.clear();
}

} // namespace

QString ImageHistogram::doc_ = getMarkdown();

ImageHistogram::ImageHistogram(QObject *parent, QQmlEngine *qml_engine, QJSEngine *js_engine)
    : core::AbstractCVTool(parent, qml_engine, js_engine)
{
    runtime_params_ = new ImageHistogramRuntimeParams(this);
}

ImageHistogram::~ImageHistogram() {}

int ImageHistogram::initInputParams()
{
    if (input_params_)
    {
        input_params_->addImage("Image", tr("图像"), tr("输入图像的路径"), QVariant(), true, true);
        if (providers_)
            providers_->addImageProvider("Image", uuid());
        input_params_->addComboBox("ColorSpace", tr("色彩空间"), tr("将输入图像转换到对应的色彩空间"), COLOR_SPACES[0],
                                   COLOR_SPACES, false, true);
    }
    return 0;
}

int ImageHistogram::initOutputParams()
{
    if (output_params_)
    {
        output_params_->addParam("ImagePath", tr("图像路径"), "", QuickToolParamType::TextParamType, QVariant(),
                                 QVariant(), false, true);
        output_params_->addParam("Channels", tr("图像通道数"), "", QuickToolParamType::IntParamType, QVariant(),
                                 QVariant(), false, true);
        output_params_->addParam("Hist", tr("直方图"), "", QuickToolParamType::Double2DArrayParamType, QVariant(),
                                 QVariant(), true, true);
        output_params_->addParam("HistMin", tr("直方图最小值"), "", QuickToolParamType::Double2DArrayParamType,
                                 QVariant(), QVariant(), true, true);
        output_params_->addParam("HistMax", tr("直方图最大值"), "", QuickToolParamType::Double2DArrayParamType,
                                 QVariant(), QVariant(), true, true);
    }
    return 0;
}

std::tuple<int, QString> ImageHistogram::doInProcess()
{
    auto algorithm_start_time = std::chrono::high_resolution_clock::now();

    auto input_params  = getInputParams();
    auto output_params = getOutputParams();

    ImageHistogramRuntimeParams *runtime = dynamic_cast<ImageHistogramRuntimeParams *>(runtime_params_);
    if (input_params == nullptr || output_params == nullptr || runtime == nullptr)
        return {-1, tr("输入/输出参数为空指针")};

    auto read_start_time = std::chrono::high_resolution_clock::now();

    cv::Mat image       = cv::imread(runtime->input.image_path.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);
    runtime->output.chs = image.channels();
    if (providers_ && providers_->size() > 0 && providers_->at(0))
        providers_->at(0)->setImage(runtime->input.image_path, image);

    auto read_end_time = std::chrono::high_resolution_clock::now();

    setProgress(0.5);

    cv::Mat mask;
    auto    roi = input_params->roi();
    if (roi && !roi->empty())
        mask = roi->toMask(image.cols, image.rows);

    cv::Mat dst;
    if (cvtColor(image, dst, runtime->input.color_space) != 0)
        return {-1, tr("转换到色彩空间 %1 失败").arg(runtime->input.color_space)};

    std::vector<cv::Mat> chs;
    cv::split(dst, chs);

    size_t size = chs.size();
    double step = 0.4 / size;
    for (size_t i = 0; i < size; ++i)
    {
        cv::Mat hist;
        const auto [hist_size, range_values] = getHistSizeAndRange(runtime->input.color_space, static_cast<int>(i));
        const auto [range_min, range_max]    = range_values;

        float        range[]  = {range_min, range_max};
        const float *ranges[] = {range};
        cv::calcHist(&chs[i], 1, 0, mask, hist, 1, &hist_size, ranges, true, false);
        //        cv::normalize(hist, hist, 0, 1, cv::NORM_MINMAX);
        float           min_value{std::numeric_limits<float>::max()};
        float           max_value{std::numeric_limits<float>::min()};
        QList<QVariant> hist_data;
        float          *data_ptr = reinterpret_cast<float *>(hist.data);
        for (int j = 0; j < hist_size; ++j)
        {
            hist_data.append(data_ptr[j]);
            min_value = std::min(min_value, data_ptr[j]);
            max_value = std::max(max_value, data_ptr[j]);
        }
        max_value = std::abs(max_value - std::numeric_limits<float>::min()) < 1e-3 ? 1. : max_value;
        min_value = std::abs(min_value - std::numeric_limits<float>::max()) < 1e-3 ? 1. : min_value;
        QList<QVariant> min_values;
        min_values.append(min_value);
        QList<QVariant> max_values;
        max_values.append(max_value);
        runtime->output.hists_data.append(hist_data);
        runtime->output.hists_min.append(min_values);
        runtime->output.hists_max.append(max_values);
        setProgress(0.5 + (i + 1) * step);
    }

    auto algorithm_end_time = std::chrono::high_resolution_clock::now();
    auto algorithm_time = std::chrono::duration<double, std::milli>(algorithm_end_time - algorithm_start_time).count();
    auto read_time      = std::chrono::duration<double, std::milli>(read_end_time - read_start_time).count();

    addAlgorithmTime(algorithm_time);
    addAlgorithmTime(read_time);
    addAlgorithmTime(algorithm_time - read_time);

    return {0, tr("运行成功")};
}

QString ImageHistogram::doc() const
{
    return doc_;
}

int ImageHistogram::cvtColor(const cv::Mat &src, cv::Mat &dst, const QString &color_space)
{
    if (src.empty())
        return -1;
    const int channels = src.channels();
    if (color_space == "RGB")
    {
        if (channels == 1)
            cv::cvtColor(src, dst, cv::COLOR_GRAY2RGB);
        else if (channels == 3)
            cv::cvtColor(src, dst, cv::COLOR_BGR2RGB);
        else if (channels == 4)
            cv::cvtColor(src, dst, cv::COLOR_BGRA2RGB);
        else
            return -1;
    }
    else if (color_space == "HSV")
    {
        if (channels == 1)
        {
            cv::cvtColor(src, dst, cv::COLOR_GRAY2BGR);
            cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);
        }
        else if (channels == 3)
            cv::cvtColor(src, dst, cv::COLOR_BGR2HSV);
        else if (channels == 4)
        {
            cv::cvtColor(src, dst, cv::COLOR_BGRA2BGR);
            cv::cvtColor(dst, dst, cv::COLOR_BGR2HSV);
        }
        else
            return -1;
    }
    else if (color_space == "Gray")
    {
        if (channels == 1)
            dst = src;
        else if (channels == 3)
            cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
        else if (channels == 4)
            cv::cvtColor(src, dst, cv::COLOR_BGRA2GRAY);
        else
            return -1;
    }
    else
        dst = src;
    return 0;
}

enum SettingsGroup
{
    Misc = core::AbstractQuickToolSettings::SettingsGroup::UserGroup + 1,
};

int ImageHistogram::initSettings()
{
    if (settings_)
    {
        // TODO: add chart height
        settings_->addGroup(SettingsGroup::Misc, tr("杂项设置"));
        settings_->addIntInputSetting(SettingsGroup::Misc, "ChartHeight", tr("图表高"), "", 400, 200, 1000);
    }
    return 0;
}

} // namespace quicktools::imgproc
