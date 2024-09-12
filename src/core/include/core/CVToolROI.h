#pragma once

#include "CoreGlobal.h"
#include "QuickToolType.h"

#include <opencv2/core.hpp>

#include <QObject>
#include <QtQml>

using quicktools::core::shapetype::ShapeType;

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT CVToolROI : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVToolROI)
    QML_UNCREATABLE("Can't not create a CVToolROI directly")
    Q_PROPERTY(QList<qreal> data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(int shapeType READ shapeType WRITE setShapeType NOTIFY shapeTypeChanged FINAL)
public:
    explicit CVToolROI(QObject *parent = nullptr);

    QList<qreal> data() const;
    void         setData(const QList<qreal> &data);

    bool empty() const;

    bool isEmpty() const
    {
        return empty();
    }

    int  shapeType() const;
    void setShapeType(const int roi_type);

    cv::Mat toMask(const int width, const int height, const int fill_value = 255) const;

private:
    QList<qreal> data_;
    int          shape_type_{ShapeType::NoShape};

signals:
    void dataChanged();
    void shapeTypeChanged();
};

} // namespace quicktools::core
