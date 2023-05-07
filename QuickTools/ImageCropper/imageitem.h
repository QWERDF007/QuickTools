#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QGraphicsPixmapItem>
#include "croprect.h"

class ImageItem : public QGraphicsPixmapItem
{
public:
    explicit ImageItem(QGraphicsItem *parent = nullptr);
    explicit ImageItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    ~ImageItem();

    void setCropRect(QRectF &rect);

    void setCropRect(const QPixmap &pixmap);

    void setPixmap(const QPixmap &pixmap);

protected:
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
