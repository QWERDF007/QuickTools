#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "macro.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

class CropRect : public QGraphicsRectItem
{
public:
    enum
    {
        Type = UserType + ItemType::CropRectType
    };

    enum Edge
    {
        TOP = 0,
        RIGHT,
        BOTTOM,
        LEFT,
    };

    enum Vertex
    {
        TOP_LEFT = 0,
        TOP_RIGHT,
        BOTTOM_RIGHT,
        BOTTOM_LEFT,
    };

    explicit CropRect(QGraphicsItem *parent = nullptr);
    explicit CropRect(const QRectF &rect, QGraphicsItem *parent = nullptr);
    explicit CropRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
    ~CropRect();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

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
    void init();

    int nearestEdge(QPointF point, qreal epsilon = 5.0);

    int nearestVertex(QPointF point, qreal epsilon = 5.0);

    qreal distanceToLine(QPointF point, QPointF p1, QPointF p2);

    void adjustRect(QPointF point);

    int selected_edge_{-1};

    int selected_vertex_{-1};
};

class ImageItem : public QGraphicsPixmapItem
{
public:
    enum
    {
        Type = UserType + ItemType::ImageItemType
    };

    explicit ImageItem(QGraphicsItem *parent = nullptr);
    explicit ImageItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    ~ImageItem();

    void setCropRect(QRectF &rect);

    void setCropRect(const QPixmap &pixmap);

    void setPixmap(const QPixmap &pixmap);

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

private slots:
};

#endif // IMAGEITEM_H
