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
        qreal new_scale =  1.0 + event->angleDelta().y() * 0.00125;
        scale(new_scale, new_scale);
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


void ImageView::updateImageItemPos()
{
    // 根据viewport重新调整位置
    qreal col = 0;
    qreal row = 0;
    qreal current_row_height = 0;
    qreal viewport_width = mapToScene(viewport()->rect()).boundingRect().width();
    for (auto item : scene()->items(Qt::AscendingOrder))
    {
        auto image_item = dynamic_cast<ImageItem*>(item);
        if (image_item != nullptr)
        {
            setImagePos(image_item, row, col, current_row_height, viewport_width);
        }
    }
}

void ImageView::setImagePos(ImageItem *item, qreal &row, qreal &col, qreal &current_row_height, qreal viewport_width)
{
    qreal item_width = item->pixmap().width();
    qreal item_height = item->pixmap().height();
    col += col == 0 ? 0 : 20;
    int new_col = col + item_width;
    int new_row = row;
    if (col && new_col > viewport_width)
    {
        new_row = row + current_row_height + 20;
        new_col = item_width;
        row = new_row;
        col = 0;
        current_row_height = item_height;
    }
    else
    {
        current_row_height = qMax(item_height, current_row_height);
    }
    item->setPos(col, row); // Set position of item
    col = new_col;
    row = new_row;
}
