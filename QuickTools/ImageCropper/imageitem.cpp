#include <QPainter>
#include <QGraphicsOpacityEffect>
#include <QStyleOptionGraphicsItem>
#include <QPushButton>

#include "imageitem.h"

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
