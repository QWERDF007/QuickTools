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
    initInputParams();
    initOutputParams();
}

int ImageHistogram::run()
{
    qInfo() << __FUNCTION__;
    auto input_params = dynamic_cast<core::AbstractCVInputParams *>(input_params_);
    if (input_params == nullptr)
    {
        return -1;
    }
    auto        param_size = input_params->rowCount();
    std::string image_path = input_params->inputImages().toString().toStdString();
    for (int i = 0; i < param_size; ++i)
    {
        auto     name = input_params->data(input_params->index(i), QuickToolParamRole::ParamNameRole).toString();
        QVariant data = input_params->data(input_params->index(i), QuickToolParamRole::ParamValueRole);
        qInfo() << __FUNCTION__ << name;
        qInfo() << __FUNCTION__ << data;
    }
    cv::Mat image = cv::imread(image_path, cv::IMREAD_UNCHANGED);
    qInfo() << __FUNCTION__ << "image size" << image.rows << image.cols << image.channels();
    std::vector<cv::Mat> chs;
    cv::split(image, chs);
    int                    hist_size[] = {32};
    float                  range[]     = {0, 255};
    const float           *ranges[]    = {range};
    QList<QList<QVariant>> hists_data;
    for (const cv::Mat &ch : chs)
    {
        cv::Mat hist;
        cv::calcHist(&ch, 1, 0, cv::noArray(), hist, 1, hist_size, ranges, true, false);
        QList<QVariant> hist_data;
        float          *data_ptr = reinterpret_cast<float *>(hist.data);
        for (int i = 0; i < hist_size[0]; ++i)
        {
            hist_data.append(data_ptr[i]);
        }
        hists_data.append(hist_data);
    }
    auto     output_params = dynamic_cast<core::AbstractCVOutputParams *>(output_params_);
    QVariant output_data   = QVariant::fromValue(hists_data);
    output_params->setData("Hist", output_data);
    output_params->setOutputImages(output_data);
    param_size = output_params->rowCount();
    for (int i = 0; i < param_size; ++i)
    {
        auto     name = output_params->data(output_params->index(i), QuickToolParamRole::ParamNameRole).toString();
        QVariant data = output_params->data(output_params->index(i), QuickToolParamRole::ParamValueRole);
        qInfo() << __FUNCTION__ << name;
        qInfo() << __FUNCTION__ << data;
    }
    return 0;
}

void ImageHistogram::initInputParams()
{
    input_params_ = new core::AbstractCVInputParams(this);
    input_params_->addParam("Image", QuickToolParamType::Text, "");
}

void ImageHistogram::initOutputParams()
{
    output_params_ = new core::AbstractCVOutputParams(this);
    output_params_->addParam("Hist", QuickToolParamType::Text, "");
}

} // namespace quicktools::imgproc
