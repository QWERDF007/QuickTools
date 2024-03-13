#include "imagehistogram.h"

#include "abstractcvtoolparam.h"

#include <opencv2/opencv.hpp>

#include <QString>
#include <QtQml>
#include <iostream>
#include <vector>

namespace quicktools::imgproc {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

ImageHistogram::ImageHistogram(QObject *parent)
    : core::AbstractCVTool(parent)
{
}

int ImageHistogram::run()
{
    qInfo() << __FUNCTION__;
//    auto input_params = dynamic_cast<core::AbstractCVInputParams *>(input_params_);
    auto input_params = inputParams();
    if (input_params == nullptr)
    {
        return -1;
    }
    auto        param_size = input_params->rowCount();
    std::string image_path;
    for (int i = 0; i < param_size; ++i)
    {
        auto     name = input_params->data(input_params->index(i), QuickToolParamRole::ParamNameRole).toString();
        QVariant data = input_params->data(input_params->index(i), QuickToolParamRole::ParamValueRole);
        qInfo() << __FUNCTION__ << name;
        qInfo() << __FUNCTION__ << data;
    }
    image_path    = input_params->data("Image", QuickToolParamRole::ParamValueRole).toString().toStdString();
    cv::Mat image = cv::imread(image_path, cv::IMREAD_UNCHANGED);
    qInfo() << __FUNCTION__ << "image size" << image.rows << image.cols << image.channels();
    std::vector<cv::Mat> chs;
    cv::split(image, chs);
    int                    hist_size[] = {255};
    float                  range[]     = {0, 255};
    const float           *ranges[]    = {range};
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
    auto     output_params = outputParams();
    QVariant output_data   = QVariant::fromValue(hists_data);
    output_params->setData("Hist", output_data);
    param_size = output_params->rowCount();
    for (int i = 0; i < param_size; ++i)
    {
        auto     name = output_params->data(output_params->index(i), QuickToolParamRole::ParamNameRole).toString();
        QVariant data = output_params->data(output_params->index(i), QuickToolParamRole::ParamValueRole);
        qInfo() << __FUNCTION__ << name;
    }
    return 0;
}

int ImageHistogram::initInputParams()
{
    if (input_params_)
    {
        input_params_->addParam("Image", QuickToolParamType::Text, true, true);
    }
    return 0;
}

int ImageHistogram::initOutputParams()
{
    if (output_params_)
    {
        output_params_->addParam("Hist", QuickToolParamType::Text, true, false);
    }
    return 0;
}

} // namespace quicktools::imgproc
