#include "CVToolROI.h"

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
    qInfo() << __FUNCTION__ << data;
    emit dataChanged();
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
    qInfo() << __FUNCTION__ << roi_type;
    emit roiTypeChanged();
}

} // namespace quicktools::core
