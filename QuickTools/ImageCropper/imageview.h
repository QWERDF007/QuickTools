#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>


class ImageView : public QGraphicsView
{
public:
    ImageView(QWidget *parent = nullptr);
    ImageView(QGraphicsScene *scene, QWidget *parent = nullptr);
    ~ImageView();

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    Q_DISABLE_COPY(ImageView)
};

#endif // IMAGEVIEW_H
