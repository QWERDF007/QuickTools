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
    NO_EDGE_SELECTED = -1,
};

enum Vertex
{
    TOP_LEFT = 0,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT,
    NO_VERTEX_SELECTED = -1,
};

static QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

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

void CropRect::setRectFromParent(const QRectF &r)
{
    setRect(mapRectFromParent(r & prect_));
    emit rectChanged(mapRectToParent(rect()));
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

QRectF CropRect::boundingRect() const
{
    // 在原有基础上向外扩充 5 个单位
    return rect().adjusted(-5,-5, 5, 5);
}

QPainterPath CropRect::shape() const
{
    // 重写以扩充鼠标事件响应范围和碰撞检测范围
    QPainterPath path;
    path.addRect(boundingRect());
    return qt_graphicsItem_shapeFromPath(path, pen());
}

void CropRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
}

void CropRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int index      = nearestVertex(event->pos(), 10);
    if (index > -1)
    {
        selected_vertex_ = index;
        selected_edge_ = NO_EDGE_SELECTED;
    }
    else
    {
        index      = nearestEdge(event->pos(), 10);
        selected_vertex_ = NO_VERTEX_SELECTED;
        selected_edge_ = index;
    }
    emit rectClicked(this, mapRectToParent(rect()));
    QGraphicsRectItem::mousePressEvent(event);
}

void CropRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ((selected_vertex_ > -1 || selected_edge_ > -1) && event->buttons() == Qt::LeftButton)
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
    int index = nearestVertex(event->pos(), 10);
    // 设置鼠标在矩形内悬浮时的样式
    if (index > -1)
    {
        switch (index)
        {
        case Vertex::TOP_LEFT:
        case Vertex::BOTTOM_RIGHT:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case Vertex::TOP_RIGHT:
        case Vertex::BOTTOM_LEFT:
            setCursor(Qt::SizeBDiagCursor);
            break;
        default:
            break;
        }
    }
    else
    {
        index = nearestEdge(event->pos(), 10);
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
    if (parent)
    {
        prect_ = parent->pixmap().rect();
    }

    selected_vertex_ = NO_VERTEX_SELECTED;
    selected_edge_ = NO_EDGE_SELECTED;
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

int CropRect::nearestVertex(QPointF point, qreal epsilon)
{
    int   index        = -1;
    qreal min_distance = std::numeric_limits<qreal>::max();
    QVector<QPointF> points{rect().topLeft(), rect().topRight(), rect().bottomRight(), rect().bottomLeft()};
    for (int i = 0; i < points.size(); ++i)
    {
        qreal distance = distanceToPoint(point, points[i]);
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

qreal CropRect::distanceToPoint(QPointF p1, QPointF p2)
{
    qreal dx = p1.x() - p2.x();
    qreal dy = p1.y() - p2.y();
    return sqrt(dx * dx + dy * dy);
}

void CropRect::adjustRect(QPointF point)
{
    if (selected_vertex_ > -1)
    {
        setRectFromParent(adjustByVertex(point));
    }
    else if (selected_edge_ > -1)
    {
        setRectFromParent(adjustByEdge(point));
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

QRectF CropRect::adjustByVertex(QPointF point)
{
    qreal x1, y1;
    qreal x2, y2;
    // 编辑矩形，不超出 parentItem 的 Pixmap
    point                = mapToParent(point);
    QPointF top_left     = mapToParent(rect().topLeft());
    QPointF bottom_right = mapToParent(rect().bottomRight());
    qreal   min_edge     = 10 * scale();

    switch (selected_vertex_) {
    case Vertex::TOP_LEFT:
    {
        x1 = qMin(point.x(), bottom_right.x() - min_edge);
        y1 = qMin(point.y(), bottom_right.y() - min_edge);
        x2 = bottom_right.x();
        y2 = bottom_right.y();
        if (!prect_.contains(QPointF(x1, y1)))
        {
            x1 = x1 < 0 ? 0 : x1;
            y1 = y1 < 0 ? 0 : y1;
        }
        break;
    }
    case Vertex::TOP_RIGHT:
    {
        x1 = top_left.x();
        y1 = qMin(point.y(), bottom_right.y() - min_edge);
        x2 = qMax(point.x(), x1 + min_edge);
        y2 = bottom_right.y();
        if (!prect_.contains(QPointF(x2, y1)))
        {
            x2 = x2 > prect_.width() ? prect_.width() : x2;
            y1 = y1 < 0 ? 0 : y1;
        }
        break;
    }
    case Vertex::BOTTOM_RIGHT:
    {
        x1 = top_left.x();
        y1 = top_left.y();
        x2 = qMax(point.x(), x1 + min_edge);
        y2 = qMax(point.y(), y1 + min_edge);
        if (!prect_.contains(QPointF(x2, y2)))
        {
            x2 = x2 > prect_.width() ? prect_.width() : x2;
            y2 = y2 > prect_.height() ? prect_.height() : y2;
        }
        break;
    }
    case Vertex::BOTTOM_LEFT:
    {
        x1 = qMin(point.x(), bottom_right.x() - min_edge);
        y1 = top_left.y();
        x2 = bottom_right.x();
        y2 = qMax(point.y(), y1 + min_edge);
        if (!prect_.contains(QPointF(x1, y2)))
        {
            x1 = x1 < 0 ? 0 : x1;
            y2 = y2 > prect_.height() ? prect_.height() : y2;
        }
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

CloseButtonItem::CloseButtonItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    setAcceptHoverEvents(true);
    setFlag(ItemIgnoresParentOpacity, true);
//    setFlag(ItemIgnoresTransformations, true);
    setZValue(100); // 设置在最上面
}

CloseButtonItem::~CloseButtonItem()
{
    qDebug() << "~CloseButtonItem";
}

QRectF CloseButtonItem::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

void CloseButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QRectF rect(0, 0, 20, 20);
    painter->fillRect(rect, QColor(140, 220, 220, 128));
    QPen pen;
    pen.setColor(QColor(255, 255, 255, 128));
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(3, 3, 17, 17);
    painter->drawLine(17, 3, 3, 17);
}

void CloseButtonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::PointingHandCursor);
}

void CloseButtonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
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

QPixmap ImageItem::cropped() const
{
    if (crop_rect_)
    {
        QRectF  r            = mapRectFromItem(crop_rect_, crop_rect_->rect());
        QRect   roundedRect  = r.toRect();
        QPixmap centerPixmap = this->pixmap().copy(roundedRect);
        return centerPixmap;
    }
    else
    {
        return QPixmap();
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

