#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>

class ImageItem;
class CropRect;

class ImageView : public QGraphicsView
{
public:
    ImageView(QWidget *parent = nullptr);

    ImageView(QGraphicsScene *scene, QWidget *parent = nullptr);

    ~ImageView();

protected:
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    Q_DISABLE_COPY(ImageView)

    void init();

    void scaleImageItem(qreal delta);

    void updateImageItemPos();

    void setImagePos(ImageItem *item, qreal &row, qreal &col, qreal &current_row_height, qreal current_viewport_width);
};

#endif // IMAGEVIEW_H
