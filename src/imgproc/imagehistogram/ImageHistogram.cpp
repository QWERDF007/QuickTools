#include "ImageHistogram.h"

#include "core/AbstractQuickToolSettings.h"

#include <opencv2/opencv.hpp>

#include <QFile>
#include <QString>
#include <QtQml>
#include <chrono>
#include <vector>

namespace quicktools::imgproc {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

const QVariantList COLOR_SPACES{
    "UNCHANGED",
    "RGB",
    "HSV",
    "Gray",
};

QString getMarkdown()
{
    QFile markdown_file("docs/imgproc/ImageHistogram.md");
    if (!markdown_file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";
    return markdown_file.readAll();
}

QString ImageHistogram::doc_ = getMarkdown();

ImageHistogram::ImageHistogram(QObject *parent)
    : core::AbstractCVTool(parent)
{
}

std::tuple<int, std::tuple<float, float>> getHistSizeAndRange(const QString &color_space, const int ch)
{
    if (color_space == "HSV" && ch == 0)
        return std::make_tuple(180, std::make_tuple(0.f, 180.f));
    else
        return std::make_tuple(256, std::make_tuple(0.f, 256.f));
}

std::tuple<int, QString> ImageHistogram::process()
{
    auto algorithm_start_time = std::chrono::high_resolution_clock::now();
    auto input_params         = getInputParams();
    auto output_params        = getOutputParams();
    if (input_params == nullptr || output_params == nullptr)
        return {-1, tr("输入/输出参数为空指针")};
    const QString image_path = input_params->data("Image", QuickToolParamRole::ParamValueRole).toString();
    if (image_path.isEmpty())
        return {-1, tr("输入图像路径为空")};
    if (!QFile::exists(image_path))
        return {-1, tr("输入图像路径不存在")};
    QString color_space = input_params->data("ColorSpace", QuickToolParamRole::ParamValueRole).toString();

    auto    read_start_time = std::chrono::high_resolution_clock::now();
    cv::Mat image           = cv::imread(image_path.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);
    auto    read_end_time   = std::chrono::high_resolution_clock::now();

    cv::Mat mask;
    auto    roi = input_params->roi();
    if (roi && !roi->empty())
        mask = roi->toMask(image.cols, image.rows);

    cv::Mat dst;
    if (cvtColor(image, dst, color_space) != 0)
        return {-1, tr("转换到色彩空间 %1 失败").arg(color_space)};

    std::vector<cv::Mat> chs;
    cv::split(dst, chs);

    QList<QVariantList> hists_data;
    QVariantList        hists_min;
    QVariantList        hists_max;
    // for (const cv::Mat &ch : chs)
    for (size_t i = 0; i < chs.size(); ++i)
    {
        cv::Mat hist;
        const auto [hist_size, range_values] = getHistSizeAndRange(color_space, static_cast<int>(i));
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
        hists_data.append(hist_data);
        hists_min.append(min_value);
        hists_max.append(max_value);
    }
    auto algorithm_end_time = std::chrono::high_resolution_clock::now();
    auto algorithm_time = std::chrono::duration<double, std::milli>(algorithm_end_time - algorithm_start_time).count();
    auto read_time      = std::chrono::duration<double, std::milli>(read_end_time - read_start_time).count();
    addAlgorithmTime(algorithm_time);
    addAlgorithmTime(read_time);
    addAlgorithmTime(algorithm_time - read_time);

    output_params->setData("Channels", image.channels());
    output_params->setData("Hist", QVariant::fromValue(hists_data));
    output_params->setData("HistMin", QVariant::fromValue(hists_min));
    output_params->setData("HistMax", QVariant::fromValue(hists_max));

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

int ImageHistogram::initInputParams()
{
    if (input_params_)
    {
        input_params_->addParam("Image", tr("图像"), tr("输入图像的路径"), QuickToolParamType::ParamImageType,
                                QVariant(), QVariant(), true, true, true, true);
        input_params_->addParam("ColorSpace", tr("色彩空间"), tr("将输入图像转换到对应的色彩空间"),
                                QuickToolParamType::ParamComboBoxType, COLOR_SPACES[0], COLOR_SPACES, false, false,
                                true, true);
    }
    return 0;
}

int ImageHistogram::initOutputParams()
{
    if (output_params_)
    {
        output_params_->addParam("Channels", tr("图像通道数"), "", QuickToolParamType::ParamIntType, QVariant(),
                                 QVariant(), false, true);
        output_params_->addParam("Hist", tr("直方图"), "", QuickToolParamType::ParamDouble2DArrayType, QVariant(),
                                 QVariant(), true, true);
        output_params_->addParam("HistMin", tr("直方图最小值"), "", QuickToolParamType::ParamDouble1DArrayType,
                                 QVariant(), QVariant(), true, true);
        output_params_->addParam("HistMax", tr("直方图最大值"), "", QuickToolParamType::ParamDouble1DArrayType,
                                 QVariant(), QVariant(), true, true);
    }
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
