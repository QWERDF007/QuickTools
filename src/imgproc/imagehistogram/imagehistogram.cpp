#include "imagehistogram.h"

#include <opencv2/highgui.hpp>

#include <QString>
#include <QtQml>
#include <vector>

namespace quicktools::imgproc {

using quicktooltypes::QuickToolParamType;

ImageHistogram::ImageHistogram(QObject *parent)
    : AbstractQuickTool(parent)
{
    initInputParams();
}

int ImageHistogram::run()
{
    qInfo() << __FUNCTION__;
    qInfo() << __FUNCTION__ << image_source_;
    auto        param_size = input_params_->rowCount();
    std::string image_path;
    for (int i = 0; i < param_size; ++i)
    {
        auto name = input_params_->data(input_params_->index(i), AbstractToolParams::ParamNameRole).toString();
        auto data = input_params_->data(input_params_->index(i), AbstractToolParams::ParamValueRole);
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

    connect(this, &AbstractQuickTool::imageSourceChanged, this, &ImageHistogram::updateImageSourceParam);
}

void ImageHistogram::updateImageSourceParam()
{
    int param_size = input_params_->rowCount();
    for (int i = 0; i < param_size; ++i)
    {
        auto index = input_params_->index(i);
        if (input_params_->data(index, AbstractToolParams::ParamNameRole) != "Image")
        {
            continue;
        }
        QString image_source = image_source_.toString();
        if (image_source.startsWith("file:///"))
        {
            image_source = image_source.remove("file:///");
        }
        input_params_->setData(index, image_source, AbstractToolParams::ParamValueRole);
        break;
    }
}

} // namespace quicktools::imgproc
