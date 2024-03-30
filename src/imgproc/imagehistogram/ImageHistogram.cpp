#include "ImageHistogram.h"

#include <opencv2/opencv.hpp>

#include <QString>
#include <QtQml>
#include <chrono>
#include <iostream>
#include <vector>

namespace quicktools::imgproc {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

ImageHistogram::ImageHistogram(QObject *parent)
    : core::AbstractCVTool(parent)
{
}

std::tuple<int, QString> ImageHistogram::run()
{
    auto input_params  = inputParams();
    auto output_params = outputParams();
    if (input_params == nullptr || output_params == nullptr)
    {
        return {-1, "输入/输出参数为空指针"};
    }

    std::string image_path = input_params->data("Image", QuickToolParamRole::ParamValueRole).toString().toLocal8Bit().toStdString();
    auto        algorithm_start_time = std::chrono::high_resolution_clock::now();

    cv::Mat image = cv::imread(image_path, cv::IMREAD_UNCHANGED);
    qInfo() << __FUNCTION__ << "image size" << image.rows << image.cols << image.channels();
    std::vector<cv::Mat> chs;
    cv::split(image, chs);
    int          hist_size[] = {255};
    float        range[]     = {0, 255};
    const float *ranges[]    = {range};

    QList<QList<QVariant>> hists_data;
    for (const cv::Mat &ch : chs)
    {
        cv::Mat hist;
        cv::calcHist(&ch, 1, 0, cv::noArray(), hist, 1, hist_size, ranges, true, false);
        cv::normalize(hist, hist, 0, 1, cv::NORM_MINMAX);
        QList<QVariant> hist_data;
        float          *data_ptr = reinterpret_cast<float *>(hist.data);
        for (int i = 0; i < hist_size[0]; ++i)
        {
            hist_data.append(data_ptr[i]);
        }
        hists_data.append(hist_data);
    }
    auto algorithm_end_time = std::chrono::high_resolution_clock::now();

    output_params->setData("Hist", QVariant::fromValue(hists_data));
    setAlgorithmTime(std::chrono::duration<double, std::milli>(algorithm_end_time - algorithm_start_time).count());
    return {0, "运行成功"};
}

int ImageHistogram::initInputParams()
{
    if (input_params_)
    {
        input_params_->addParam("Image", "图像", QuickToolParamType::ParamImageType, QVariant(), QVariant(), true, true, true, true);
        input_params_->setIsInit(true);
    }
    return 0;
}

int ImageHistogram::initOutputParams()
{
    if (output_params_)
    {
        output_params_->addParam("Hist", "直方图", QuickToolParamType::ParamDouble2DArrayType, QVariant(), QVariant(), true, true);
        output_params_->setIsInit(true);
    }
    return 0;
}

} // namespace quicktools::imgproc
