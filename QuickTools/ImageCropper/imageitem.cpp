#include "imageitem.h"

#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <numeric>

enum Edge
{
    LEFT = 0,
    TOP,
    RIGHT,
    BOTTOM,
};

enum Vertex
{
    TOP_LEFT = 0,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT,
};

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
        QRectF  prect   = parent->pixmap().rect();
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
    int index      = nearestEdge(event->pos(), 10);
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
    selected_edge_   = -1;
    selected_vertex_ = -1;
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void CropRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    int index = nearestEdge(event->pos(), 10);
    // 设置鼠标在矩形内悬浮时的样式
    if (index > -1)
    {
        switch (index)
        {
        case Edge::LEFT:
        case Edge::RIGHT:
            setCursor(Qt::SizeHorCursor);
            break;
        case Edge::TOP:
        case Edge::BOTTOM:
            setCursor(Qt::SizeVerCursor);
            break;
        default:
            break;
        }
    }
    else
    {
        setCursor(Qt::SizeAllCursor);
    }
    QGraphicsRectItem::hoverMoveEvent(event);
}

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
    int   index        = -1;
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
            index        = i;
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
    prect_                      = parent->pixmap().rect();
    if (selected_vertex_ > -1)
    {
    }
    else if (selected_edge_ > -1)
    {
        QRectF r = adjustByEdge(point);
        // 这样后续移动才不会有问题
        setRect(QRectF(0, 0, r.width(), r.height()));
        setPos(r.topLeft());
    }
    else
    {
        // fixme
    }
}

QRectF CropRect::adjustByEdge(QPointF point)
{
    qreal x, y;
    qreal width, height;
    point                = mapToParent(point);
    QPointF top_left     = mapToParent(rect().topLeft());
    QPointF bottom_right = mapToParent(rect().bottomRight());
    qreal   min_edge     = 10 * scale();

    switch (selected_edge_)
    {
    case Edge::TOP:
    {
        x = top_left.x();
        y = qMin(point.y(), bottom_right.y() - min_edge);
        if (!prect_.contains(QPointF(x, y)))
        {
            y = 0;
        }
        width  = rect().width();
        height = bottom_right.y() - y;
        break;
    }
    case Edge::RIGHT:
    {
        x           = top_left.x();
        y           = top_left.y();
        qreal right = qMax(point.x(), top_left.x() + min_edge);
        if (!prect_.contains(QPointF(right, y)))
        {
            right = prect_.width();
        }
        width  = right - x;
        height = rect().height();
        break;
    }
    case Edge::BOTTOM:
    {
        x            = top_left.x();
        y            = top_left.y();
        qreal bottom = qMax(point.y(), top_left.y() + min_edge);
        if (!prect_.contains(QPointF(x, bottom)))
        {
            bottom = prect_.height();
        }
        width  = rect().width();
        height = bottom - y;
        break;
    }
    case Edge::LEFT:
    {
        x = qMin(point.x(), bottom_right.x() - min_edge);
        y = top_left.y();
        if (!prect_.contains(QPointF(x, y)))
        {
            x = 0;
        }
        width  = bottom_right.x() - x;
        height = rect().height();
        break;
    }
    default:
    {
        x = y = width = height = 0;
        break;
    }
    }
    return QRectF(x, y, width, height);
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
}

void ImageItem::setCropRect(const QPixmap &pixmap)
{
    if (crop_rect_ == nullptr)
    {
        int    min_edge = qMin(pixmap.width(), pixmap.height());
        QRectF rect(0, 0, min_edge, min_edge);
        crop_rect_ = new CropRect(rect, this);
    }
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
    QRectF  r            = mapRectFromItem(crop_rect_, crop_rect_->rect());
    QRectF  roundedRect  = QRectF(round(r.x()), round(r.y()), round(r.width()), round(r.height()));
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
