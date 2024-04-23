#pragma once

#include <QObject>
#include <QtQml>

namespace quicktools::core {

class QuickToolROI : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(QuickToolROI)
    QML_UNCREATABLE("Can't not create a QuickToolROI directly")

    Q_PROPERTY(QList<qreal> data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(ROITYpe roiType READ roiType WRITE setROIType NOTIFY roiTypeChanged FINAL)
public:
    explicit QuickToolROI(QObject *parent = nullptr);

    QList<qreal> data() const;
    void setData(const QList<qreal>& data);

    enum ROITYpe {NoROI, Rectangle, Circle, Polygon};
    Q_ENUM(ROITYpe);

    ROITYpe roiType() const;
    void setROIType(const ROITYpe roi_type);

private:
    QList<qreal> data_;
    ROITYpe roi_type_{ROITYpe::NoROI};

signals:
    void dataChanged();
    void roiTypeChanged();
};

} // namespace quicktools::core
