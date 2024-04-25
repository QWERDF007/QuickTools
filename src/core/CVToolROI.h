#pragma once

#include "CoreGlobal.h"

#include <opencv2/core.hpp>

#include <QObject>
#include <QtQml>


namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT CVToolROI : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVToolROI)
    QML_UNCREATABLE("Can't not create a CVToolROI directly")
    Q_PROPERTY(QList<qreal> data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(ROITYpe roiType READ roiType WRITE setROIType NOTIFY roiTypeChanged FINAL)
public:
    explicit CVToolROI(QObject *parent = nullptr);

    QList<qreal> data() const;
    void         setData(const QList<qreal> &data);

    bool empty() const;

    bool isEmpty() const
    {
        return empty();
    }

    // clang-format off
    enum ROITYpe { NoROI, Rectangle, Circle, Polygon };
    Q_ENUM(ROITYpe);
    // clang-format on

    ROITYpe roiType() const;
    void    setROIType(const ROITYpe roi_type);

    cv::Mat toMask(const int width, const int height, const int fill_value = 255) const;

private:
    QList<qreal> data_;
    ROITYpe      roi_type_{ROITYpe::NoROI};

signals:
    void dataChanged();
    void roiTypeChanged();
};

} // namespace quicktools::core
