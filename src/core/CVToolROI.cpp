#include "CVToolROI.h"

#include <opencv2/imgproc.hpp>

namespace quicktools::core {

CVToolROI::CVToolROI(QObject *parent)
    : QObject(parent)
    , roi_type_(ROITYpe::NoROI)
{
}

QList<qreal> CVToolROI::data() const
{
    return data_;
}

void CVToolROI::setData(const QList<qreal> &data)
{
    data_ = data;
    emit dataChanged();
}

bool CVToolROI::empty() const
{
    return roi_type_ == ROITYpe::NoROI || data_.size() == 0;
}

CVToolROI::ROITYpe CVToolROI::roiType() const
{
    return roi_type_;
}

void CVToolROI::setROIType(const ROITYpe roi_type)
{
    if (roi_type == roi_type_)
        return;
    roi_type_ = roi_type;
    emit roiTypeChanged();
}

cv::Mat CVToolROI::toMask(const int width, const int height, const int fill_value) const
{
    cv::Mat mask;
    if (roi_type_ == ROITYpe::Rectangle && data_.size() >= 4)
    {
        mask = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));
        cv::rectangle(mask, cv::Rect(data_[0], data_[1], data_[2], data_[3]), cv::Scalar(fill_value), cv::FILLED);
    }
    else if (roi_type_ == ROITYpe::Circle && data_.size() >= 3)
    {
        mask = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));
        cv::circle(mask, cv::Point(data_[0], data_[1]), data_[2], cv::Scalar(fill_value), cv::FILLED);
    }
    else if (roi_type_ == ROITYpe::Polygon)
    {
        // TODO
    }
    return mask;
}

} // namespace quicktools::core
