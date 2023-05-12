#include "imageitem.h"

#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPushButton>
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
    // 限制移动不超出 parentItem 的 Pixmap
    QGraphicsPixmapItem *parent = qgraphicsitem_cast<ImageItem *>(parentItem());
    if (parent && change == QGraphicsItem::ItemPositionChange)
    {
        QPointF new_pos = value.toPointF();
        QPointF pos_on_pixmap = new_pos + rect().topLeft(); // 加上 topLeft 才是在 pixmap 上的位置
        QRectF  prect   = parent->pixmap().rect();
        if (!prect.contains(pos_on_pixmap) || !prect.contains(pos_on_pixmap + QPointF(rect().width(), rect().height())))
        {
            new_pos.setX(qMin(prect.right() - rect().left() - rect().width(), qMax(new_pos.x(), prect.left() - rect().left())));
            new_pos.setY(qMin(prect.bottom() - rect().top() - rect().height(), qMax(new_pos.y(), prect.top() - rect().top())));
            emit rectChanged(mapRectToParent(rect()));
            return new_pos;
        }
        emit rectChanged(mapRectToParent(rect()));
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
    emit rectClicked(this, mapRectToParent(rect()));
    QGraphicsRectItem::mousePressEvent(event);
}

void CropRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (selected_edge_ > -1 && event->buttons() == Qt::LeftButton)
    {
        adjustRect(event->pos());
        emit rectChanged(mapRectToParent(rect()));
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

    QGraphicsPixmapItem *parent = qgraphicsitem_cast<ImageItem *>(parentItem());
    prect_                      = parent->pixmap().rect();
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

qreal CropRect::distanceToPoint(QPointF p1, QPointF p2)
{
    return 0;
}

void CropRect::adjustRect(QPointF point)
{
    if (selected_vertex_ > -1)
    {
    }
    else if (selected_edge_ > -1)
    {
        // 在内部调用 setRect 需要 item 坐标
        QRectF r = adjustByEdge(point);
        setRect(mapRectFromParent(r));
    }
    else
    {
        // fixme
    }
}

QRectF CropRect::adjustByEdge(QPointF point)
{
    qreal x1, y1;
    qreal x2, y2;
    // 编辑矩形，不超出 parentItem 的 Pixmap
    point                = mapToParent(point);
    QPointF top_left     = mapToParent(rect().topLeft());
    QPointF bottom_right = mapToParent(rect().bottomRight());
    qreal   min_edge     = 10 * scale();
    switch (selected_edge_)
    {
    case Edge::TOP:
    {
        x1 = top_left.x();
        y1 = qMin(point.y(), bottom_right.y() - min_edge);
        if (!prect_.contains(QPointF(x1, y1)))
        {
            y1 = 0;
        }
        x2 = bottom_right.x();
        y2 = bottom_right.y();
        break;
    }
    case Edge::RIGHT:
    {
        x1          = top_left.x();
        y1          = top_left.y();
        qreal right = qMax(point.x(), top_left.x() + min_edge);
        if (!prect_.contains(QPointF(right, y1)))
        {
            right = prect_.width();
        }
        x2 = right;
        y2 = bottom_right.y();
        break;
    }
    case Edge::BOTTOM:
    {
        x1           = top_left.x();
        y1           = top_left.y();
        qreal bottom = qMax(point.y(), top_left.y() + min_edge);
        if (!prect_.contains(QPointF(x1, bottom)))
        {
            bottom = prect_.height();
        }
        x2 = bottom_right.x();
        y2 = bottom;
        break;
    }
    case Edge::LEFT:
    {
        x1 = qMin(point.x(), bottom_right.x() - min_edge);
        y1 = top_left.y();
        if (!prect_.contains(QPointF(x1, y1)))
        {
            x1 = 0;
        }
        x2 = bottom_right.x();
        y2 = bottom_right.y();
        break;
    }
    default:
    {
        x1 = y1 = x2 = y2 = 0;
        break;
    }
    }
    return QRectF(QPointF(x1, y1), QPointF(x2, y2)) & prect_;
}

ImageItem::ImageItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
}

ImageItem::ImageItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
    setAcceptHoverEvents(true);
}

ImageItem::~ImageItem()
{
    qDebug() << "~ImageItem";
}

void ImageItem::setCropRect(CropRect *crop_rect)
{
    if (crop_rect)
    {
        crop_rect_ = crop_rect;
    }
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
    // fixme: 放大后非矩形区域有残留， FullViewPortUpdate 可以解决，但是不知道能不能通过其他解决
    // fixme: toRect 方法会在不同 x, y 产生不同的 width, height
    if (crop_rect_)
    {
        QRectF  r            = mapRectFromItem(crop_rect_, crop_rect_->rect());
        QRect   roundedRect  = r.toRect();
        QPixmap centerPixmap = pixmap.copy(roundedRect);

        painter->setOpacity(1);
        painter->drawPixmap(roundedRect.topLeft(), centerPixmap);
    }
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
