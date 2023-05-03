#include <QPainter>

#include "croprect.h"
#include <QGraphicsPixmapItem>

CropRect::CropRect(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{

}

CropRect::CropRect(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent)
{

}

CropRect::CropRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, w, h, parent)
{

}

CropRect::~CropRect()
{

}

QVariant CropRect::itemChange(GraphicsItemChange change, const QVariant &value)
{
    QGraphicsPixmapItem *parent = qgraphicsitem_cast<QGraphicsPixmapItem *>(parentItem());
    if (parent && change == QGraphicsItem::ItemPositionChange) {
        QPointF new_pos = value.toPointF();
        QRectF prect = parent->pixmap().rect();
        if (!prect.contains(new_pos) || !prect.contains(new_pos + QPointF(rect().width(), rect().height()))) {
            new_pos.setX(qMin(prect.right() - rect().width(), qMax(new_pos.x(), prect.left())));
            new_pos.setY(qMin(prect.bottom() - rect().height(), qMax(new_pos.y(), prect.top())));
            return new_pos;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void CropRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QColor(0,255,0,128));
//    painter->setBrush(QColor(0,255,0,128));
    QGraphicsRectItem::paint(painter, option, widget);
}
