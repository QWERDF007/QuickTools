#include <QPainter>

#include "imageitem.h"

ImageItem::ImageItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{

}

ImageItem::ImageItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
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
//    QRectF rect = boundingRect();
    QPixmap pixmap = this->pixmap();
//    qDebug() << "draw pixmap";
//    painter->drawPixmap(QGraphicsPixmapItem::offset(), pixmap);
//QGraphicsOpacityEffect
    QRectF r = mapRectFromItem(crop_rect_, crop_rect_->rect());
    QPixmap centerPixmap = pixmap.copy(r.toRect());
//    painter->fillRect(centerRect, QColor(0, 0, 0, 128));
    painter->drawPixmap(r.topLeft(), centerPixmap);
}
