#include <QPainter>

#include "croprect.h"
#include <QGraphicsPixmapItem>

CropRect::CropRect(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    init();
}

CropRect::CropRect(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent)
{
    init();
}

CropRect::CropRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, w, h, parent)
{
    init();
}

CropRect::~CropRect()
{
    qDebug() << "~CropRect";
}

QVariant CropRect::itemChange(GraphicsItemChange change, const QVariant &value)
{
    // 限制移动不超出 Pixmap
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
    QGraphicsRectItem::paint(painter, option, widget);
}

void CropRect::init()
{
    setPen(Qt::NoPen);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
