#include "QuickToolROI.h"

namespace quicktools::core {

QuickToolROI::QuickToolROI(QObject *parent)
    : QObject(parent)
    , roi_type_(ROITYpe::NoROI)
{
}

QList<qreal> QuickToolROI::data() const
{
    return data_;
}

void QuickToolROI::setData(const QList<qreal> &data)
{
    data_ = data;
    qInfo() << __FUNCTION__ << data;
    emit dataChanged();
}

QuickToolROI::ROITYpe QuickToolROI::roiType() const
{
    return roi_type_;
}

void QuickToolROI::setROIType(const ROITYpe roi_type)
{
    if (roi_type == roi_type_)
        return;
    roi_type_ = roi_type;
    qInfo() << __FUNCTION__ << roi_type;
    emit roiTypeChanged();
}

} // namespace quicktools::core
