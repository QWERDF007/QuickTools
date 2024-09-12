#include "core/CVToolROI.h"

#include <opencv2/imgproc.hpp>

namespace quicktools::core {

CVToolROI::CVToolROI(QObject *parent)
    : QObject(parent)
    , shape_type_(ShapeType::NoShape)
{
}

QList<qreal> CVToolROI::data() const
{
    return data_;
}

void CVToolROI::setData(const QList<qreal> &data)
{
    if (shape_type_ == ShapeType::Rectangle && data.size() == 4)
        data_ = data;
    else if (shape_type_ == ShapeType::Circle && data.size() == 3)
        data_ = data;
    else if (shape_type_ == ShapeType::Polygon && data.size() >= 3)
        data_ = data;
    else
    {
        shape_type_ = ShapeType::NoShape;
        data_.clear();
        emit shapeTypeChanged();
    }
    emit dataChanged();
}

bool CVToolROI::empty() const
{
    return shape_type_ == ShapeType::NoShape || data_.size() == 0;
}

int CVToolROI::shapeType() const
{
    return shape_type_;
}

void CVToolROI::setShapeType(const int shape_type)
{
    if (shape_type == shape_type_)
        return;
    shape_type_ = shape_type;
    emit shapeTypeChanged();
}

cv::Mat CVToolROI::toMask(const int width, const int height, const int fill_value) const
{
    cv::Mat mask;
    if (shape_type_ == ShapeType::Rectangle && data_.size() >= 4)
    {
        mask = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));
        cv::rectangle(mask, cv::Rect(data_[0], data_[1], data_[2], data_[3]), cv::Scalar(fill_value), cv::FILLED);
    }
    else if (shape_type_ == ShapeType::Circle && data_.size() >= 3)
    {
        mask = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));
        cv::circle(mask, cv::Point(data_[0], data_[1]), data_[2], cv::Scalar(fill_value), cv::FILLED);
    }
    else if (shape_type_ == ShapeType::Polygon)
    {
        // TODO
    }
    return mask;
}

} // namespace quicktools::core
