#include "imagehistogram.h"

#include <opencv2/highgui.hpp>

#include <QString>
#include <QtQml>
#include <vector>

namespace quicktools::imgproc {

using quicktooltypes::QuickToolParamRole;
using quicktooltypes::QuickToolParamType;

ImageHistogram::ImageHistogram(QObject *parent)
    : AbstractQuickTool(parent)
{
    initInputParams();
}

int ImageHistogram::run()
{
    qInfo() << __FUNCTION__;
    auto        param_size = input_params_->rowCount();
    std::string image_path;
    for (int i = 0; i < param_size; ++i)
    {
        auto     name = input_params_->data(input_params_->index(i), QuickToolParamRole::ParamNameRole).toString();
        QVariant data = input_params_->data(input_params_->index(i), QuickToolParamRole::ParamValueRole);
        if (name == "Image")
        {
            image_path = data.toString().toStdString();
        }
        qInfo() << __FUNCTION__ << data;
    }
    cv::Mat image = cv::imread(image_path, cv::IMREAD_UNCHANGED);
    qInfo() << __FUNCTION__ << "image size" << image.rows << image.cols << image.channels();
    std::vector<cv::Mat> bgr_planes;
    cv::split(image, bgr_planes);
    return 0;
}

void ImageHistogram::initInputParams()
{
    input_params_ = new AbstractToolInputParams(this);
    input_params_->addParam("Image", QuickToolParamType::Text, "");
}

} // namespace quicktools::imgproc
