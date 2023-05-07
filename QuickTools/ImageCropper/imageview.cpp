#include <QWheelEvent>

#include "imageview.h"


ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent)
{

}

ImageView::ImageView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{

}

ImageView::~ImageView()
{

}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        qreal new_scale =  1.0 + event->angleDelta().y() * 0.00125;
        scale(new_scale, new_scale);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}


