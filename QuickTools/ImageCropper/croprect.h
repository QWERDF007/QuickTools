
#ifndef CROPRECT_H
#define CROPRECT_H


#include <QGraphicsRectItem>


class CropRect : public QGraphicsRectItem
{
public:
    enum { Type = UserType + 1 };
    explicit CropRect(QGraphicsItem *parent = nullptr);
    explicit CropRect(const QRectF &rect, QGraphicsItem *parent = nullptr);
    explicit CropRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr);
    ~CropRect();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    int type() const override { return Type; }

private:

};

#endif // CROPRECT_H
