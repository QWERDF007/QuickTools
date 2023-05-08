#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QStyleOptionGraphicsItem>

#include "imageitem.h"


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

void CropRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // 设置鼠标在矩形内悬浮时的样式
    setCursor(Qt::SizeAllCursor);
    QGraphicsRectItem::hoverEnterEvent(event);
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
        int min_edge = qMin(pixmap.width(), pixmap.height());
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

    QPixmap pixmap = this->pixmap();
    // 半透明绘制pixmap
    painter->setOpacity(opacity_);
    painter->drawPixmap(QGraphicsPixmapItem::offset(), pixmap);

    // 绘制矩形区域
    // 防止矩形抖动和子像素渲染伪影
    QRectF r = mapRectFromItem(crop_rect_, crop_rect_->rect());
    QRectF roundedRect = QRectF(round(r.x()), round(r.y()), r.width(), r.height());
    QPixmap centerPixmap = pixmap.copy(roundedRect.toRect());
    painter->setOpacity(1);
    painter->drawPixmap(roundedRect.topLeft(), centerPixmap);
}

void ImageItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // 鼠标进入时修改透明度，突出显示
    opacity_ = 0.5;
    update();
}

void ImageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // 鼠标离开时修改透明度，突出显示
    opacity_ = 0.3;
    update();
}
