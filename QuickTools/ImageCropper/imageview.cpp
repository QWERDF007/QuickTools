#include <QWheelEvent>

#include "imageview.h"
#include "imageitem.h"


ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent)
{
    init();
}

ImageView::ImageView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    init();
}

ImageView::~ImageView()
{

}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
//        qreal new_scale =  1.0 + event->angleDelta().y() * 0.00125;

//        scale(new_scale, new_scale);
        scaleImageItem(event->angleDelta().y() * 0.00125);
        updateImageItemPos();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void ImageView::resizeEvent(QResizeEvent *event)
{
    updateImageItemPos();
    QGraphicsView::resizeEvent(event);
}

void ImageView::init()
{
    // 缩放后左上对齐
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
}

void ImageView::scaleImageItem(qreal delta)
{
    qreal col = 0;
    qreal row = 0;
    qreal current_row_height = 0;
    for (auto item : scene()->items(Qt::AscendingOrder))
    {
        auto image_item = dynamic_cast<ImageItem*>(item);
        if (image_item != nullptr)
        {
            qreal new_scale = item->scale() + delta;
            if (new_scale < 0.2 || new_scale > 5)
            {
                continue;
            }
            image_item->setScale(item->scale() + delta);
            setImagePos(image_item, row, col, current_row_height);
        }
    }
}

void ImageView::updateImageItemPos()
{
    qreal col = 0;
    qreal row = 0;
    qreal current_row_height = 0;
    for (auto item : scene()->items(Qt::AscendingOrder))
    {
        auto image_item = dynamic_cast<ImageItem*>(item);
        if (image_item != nullptr)
        {
            setImagePos(image_item, row, col, current_row_height);
        }
    }
}

void ImageView::setImagePos(ImageItem *item, qreal &row, qreal &col, qreal &current_row_height)
{
    auto factor = item->scale();
    auto scaled_width = item->pixmap().width() * factor;
    auto scaled_height = item->pixmap().height() * factor;
    col += col == 0 ? 0 : 20;
    int new_col = col + scaled_width;
    int new_row = row;
    if (col && new_col > width())
    {
        col = 0;
        row += current_row_height + 20;
        new_col = scaled_width;
        new_row = row;
        current_row_height = scaled_height;
    }
    else
    {
        current_row_height = qMax(scaled_height, current_row_height);
    }

    item->setPos(col, row); // Set position of item
    col = new_col;
    row = new_row;
}
