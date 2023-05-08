#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

#include "macro.h"


class CropRect : public QGraphicsRectItem
{
public:
    enum { Type = UserType + ItemType::CropRectType };
    explicit CropRect(QGraphicsItem *parent = nullptr);
    explicit CropRect(const QRectF &rect, QGraphicsItem *parent = nullptr);
    explicit CropRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr);
    ~CropRect();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    int type() const override { return Type; }

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void init();

};


class ImageItem : public QGraphicsPixmapItem
{
public:
    enum { Type = UserType + ItemType::ImageItemType };
    explicit ImageItem(QGraphicsItem *parent = nullptr);
    explicit ImageItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    ~ImageItem();

    void setCropRect(QRectF &rect);

    void setCropRect(const QPixmap &pixmap);

    void setPixmap(const QPixmap &pixmap);

protected:
    int type() const override { return Type; }

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
