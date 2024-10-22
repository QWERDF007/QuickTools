#pragma once

#include "CoreGlobal.h"

#include <opencv2/core.hpp>

#include <QMetaType>
#include <QObject>
#include <QtQml>

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT CVToolShape : public QObject
{
    Q_OBJECT
    // QML_NAMED_ELEMENT(CVToolShape)
    // QML_UNCREATABLE("Can't not create a CVToolShape directly")
    QML_ANONYMOUS
    Q_PROPERTY(QList<qreal> data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(ShapeType shapeType READ shapeType WRITE setShapeType NOTIFY shapeTypeChanged FINAL)
public:
    explicit CVToolShape(QObject *parent = nullptr);
    CVToolShape(const CVToolShape &other);
    CVToolShape &operator=(const CVToolShape &other);

    QList<qreal> data() const
    {
        return data_;
    }

    void setData(const QList<qreal> &data);

    bool empty() const
    {
        return shape_type_ == ShapeType::NoShape || data_.size() == 0;
    }

    bool isEmpty() const
    {
        return empty();
    }

    enum ShapeType
    {
        NoShape   = 0x00000000, //!< 没有任何形状
        Rectangle = 0x00000001, //!< 矩形
        Circle    = 0x00000002, //!< 圆形
        Polygon   = 0x00000004, //!< 多边形
        AllShapes = 0x07ffffff, //!< 所有形状
    };
    Q_ENUM(ShapeType)

    ShapeType shapeType() const
    {
        return shape_type_;
    }

    void setShapeType(const ShapeType shape_type);

    void clear();

protected:
    QList<qreal> data_;
    ShapeType    shape_type_{ShapeType::NoShape};

signals:
    void dataChanged();
    void shapeTypeChanged();
};

class QUICKTOOLS_CORE_EXPORT CVToolShapeListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    CVToolShapeListModel(QObject *parent = nullptr);
    ~CVToolShapeListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    bool empty() const
    {
        return rowCount() <= 0;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool     setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setShapes(const QList<CVToolShape> &shapes);

    enum Role
    {
        ShapeType = Qt::UserRole + 1,
        ShapeData,
    };

    QHash<int, QByteArray> roleNames() const override;

    const QList<CVToolShape> &shapes() const
    {
        return shapes_;
    }

private:
    QList<CVToolShape> shapes_;

private slots:
    void onShapesChanged(QList<CVToolShape> shapes);

signals:
    void shapesChanged(QList<CVToolShape>);
};

class QUICKTOOLS_CORE_EXPORT CVToolROI : public CVToolShape
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVToolROI)
    QML_UNCREATABLE("Can't not create a CVToolROI directly")
public:
    explicit CVToolROI(QObject *parent = nullptr);

    cv::Mat  toMask(const int width, const int height, const int fill_value = 255) const;
    cv::Rect toRect() const;
};

} // namespace quicktools::core
