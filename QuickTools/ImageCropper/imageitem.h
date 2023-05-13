#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "macro.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QObject>

class CropRect
    : public QObject
    , public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum
    {
        Type = UserType + ItemType::CropRectType
    };

    explicit CropRect(QGraphicsItem *parent = nullptr);
    explicit CropRect(const QRectF &rect, QGraphicsItem *parent = nullptr);
    explicit CropRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
    ~CropRect();

    void setRectFromParent(const QRectF &r);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    int type() const override
    {
        return Type;
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    //    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    Q_DISABLE_COPY(CropRect)

    void init();

    int nearestEdge(QPointF point, qreal epsilon = 5.0);

    int nearestVertex(QPointF point, qreal epsilon = 5.0);

    qreal distanceToLine(QPointF point, QPointF p1, QPointF p2);

    qreal distanceToPoint(QPointF p1, QPointF p2);

    void adjustRect(QPointF point);

    QRectF adjustByEdge(QPointF point);

    QRectF adjustByVertex(QPointF point);

    int selected_edge_{-1};

    int selected_vertex_{-1};

    QRectF prect_;

signals:
    void rectChanged(QRectF);

    void rectClicked(CropRect *, QRectF);
};

class CloseButtonItem : public QGraphicsItem
{
public:
    enum { Type = UserType + ItemType::CloseButtonType };
    explicit CloseButtonItem(QGraphicsItem *parent = nullptr);
    ~CloseButtonItem();

protected:
    int type() const override
    {
        return Type;
    }

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

};

class ImageItem
    : public QObject
    , public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum
    {
        Type = UserType + ItemType::ImageItemType
    };

    explicit ImageItem(QGraphicsItem *parent = nullptr);
    explicit ImageItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    ~ImageItem();

    void setCropRect(CropRect *crop_rect);

    QPixmap cropped() const;

protected:
    int type() const override
    {
        return Type;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    Q_DISABLE_COPY(ImageItem)

    CropRect *crop_rect_{nullptr};

    qreal opacity_ = 0.3;
};

#endif // IMAGEITEM_H
