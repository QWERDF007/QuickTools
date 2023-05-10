#include "imageitem.h"

#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <numeric>

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
    QGraphicsPixmapItem *parent = qgraphicsitem_cast<ImageItem *>(parentItem());
    if (parent && change == QGraphicsItem::ItemPositionChange)
    {
        QPointF new_pos = value.toPointF();
        QRectF  prect = parent->pixmap().rect();
        if (!prect.contains(new_pos) || !prect.contains(new_pos + QPointF(rect().width(), rect().height())))
        {
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

void CropRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int index = nearestEdge(event->pos(), 5);
    selected_edge_ = index;
    QGraphicsRectItem::mousePressEvent(event);
}

void CropRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (selected_edge_ > -1 && event->buttons() == Qt::LeftButton)
    {
        adjustRect(event->pos());
    }
    else
    {
        QGraphicsRectItem::mouseMoveEvent(event);
    }
}

void CropRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    selected_edge_ = -1;
    selected_vertex_ = -1;
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void CropRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    int index = nearestEdge(event->pos(), 5);
    // 设置鼠标在矩形内悬浮时的样式
    if (index > -1)
    {
        setCursor(Qt::SizeHorCursor);
    }
    else
    {
        setCursor(Qt::SizeAllCursor);
    }
    QGraphicsRectItem::hoverMoveEvent(event);
}

//void CropRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
//{
//    int index = nearestEdge(event->pos());
//    qDebug() << event->pos() << "nearestEdge" << index;
//    // 设置鼠标在矩形内悬浮时的样式
//    if (index > -1)
//    {
//        setCursor(Qt::SizeHorCursor);
//    }
//    else
//    {
//        setCursor(Qt::SizeAllCursor);
//    }
//    QGraphicsRectItem::hoverEnterEvent(event);
//}

void CropRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // 设置鼠标悬浮在矩形外时的样式
    setCursor(Qt::ArrowCursor);
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void CropRect::init()
{
    // 不绘制边框
    setPen(Qt::NoPen);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
}

int CropRect::nearestEdge(QPointF point, qreal epsilon)
{
    int index = -1;

    qreal min_distance = std::numeric_limits<qreal>::max();

    QVector<QPointF> points{rect().topLeft(), rect().topRight(), rect().bottomRight(), rect().bottomLeft()};
    for (int i = 0; i < points.size(); ++i)
    {
        int p1 = i - 1 < 0 ? points.size() - 1 : i - 1;
        int p2 = i;

        qreal distance = distanceToLine(point, points[p1], points[p2]);
        if (distance < epsilon && distance < min_distance)
        {
            min_distance = distance;
            index = i;
        }
    }
    return index;
}

qreal CropRect::distanceToLine(QPointF point, QPointF p1, QPointF p2)
{
    qreal A = p2.y() - p1.y();
    qreal B = p1.x() - p2.x();
    qreal C = p2.x() * p1.y() - p1.x() * p2.y();
    return abs(A * point.x() + B * point.y() + C) / sqrt(pow(A, 2) + pow(B, 2));
}

void CropRect::adjustRect(QPointF point)
{
    QGraphicsPixmapItem *parent = qgraphicsitem_cast<ImageItem *>(parentItem());

    QRectF prect = parent->pixmap().rect();
    qreal  right = qMax(point.x(), rect().left());
    qreal  width = right - rect().left();
    switch (selected_edge_)
    {
    case Edge::TOP:
    {
        break;
    }

    case Edge::RIGHT:
    {
        break;
    }
    default:
    {
        break;
    }
    }

    if (!prect.contains(QPointF(width, rect().height())))
    {
        width = prect.width();
    }
    setRect(QRectF(0, 0, width, rect().height()));
}

ImageItem::ImageItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
}

ImageItem::ImageItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
    setAcceptHoverEvents(true);
    setCropRect(pixmap);
}

ImageItem::~ImageItem()
{
    qDebug() << "~ImageItem";
}

void ImageItem::setCropRect(QRectF &rect)
{
    //    CropRect *old_rect = crop_rect_;
    crop_rect_ = new CropRect(0, 0, rect.width(), rect.height(), this);
    crop_rect_->setPos(rect.x(), rect.y());
    qDebug() << crop_rect_;
}

void ImageItem::setCropRect(const QPixmap &pixmap)
{
    if (crop_rect_ == nullptr)
    {
        int    min_edge = qMin(pixmap.width(), pixmap.height());
        QRectF rect(0, 0, min_edge, min_edge);
        crop_rect_ = new CropRect(rect, this);
    }
    qDebug() << crop_rect_;
}

void ImageItem::setPixmap(const QPixmap &pixmap)
{
    setCropRect(pixmap);
    QGraphicsPixmapItem::setPixmap(pixmap);
}

void ImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPixmap pixmap = this->pixmap();
    // 半透明绘制pixmap
    painter->setOpacity(opacity_);
    painter->drawPixmap(QGraphicsPixmapItem::offset(), pixmap);

    // 绘制矩形区域
    // 防止矩形抖动和子像素渲染伪影
    QRectF  r = mapRectFromItem(crop_rect_, crop_rect_->rect());
    QRectF  roundedRect = QRectF(round(r.x()), round(r.y()), r.width(), r.height());
    QPixmap centerPixmap = pixmap.copy(roundedRect.toRect());
    painter->setOpacity(1);
    painter->drawPixmap(roundedRect.topLeft(), centerPixmap);
}

void ImageItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // 鼠标进入时修改透明度，突出显示
    opacity_ = 0.5;
    update();
    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void ImageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // 鼠标离开时修改透明度，突出显示
    opacity_ = 0.3;
    update();
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}
