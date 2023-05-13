
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QThreadPool>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class CropRect;
class ImageItem;

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newImage(ImageItem *);

private:
    void readImage(const QFileInfoList &filelist);

    void setCropRectInfo(QRectF rect);

    void setCropRect(QRect rect);

    void enableSpinBox();

    void disableSpinBox();

    void rectEditingFinished();

    Ui::MainWindow *ui;

    QGraphicsScene scene_;

    QThreadPool *pool_{nullptr};

    CropRect *selected_crop_rect_{nullptr};

    QMap<ImageItem *, QString> image_fpaths_;

private slots:
    void openFolder();
    void addImage(ImageItem *item);
    void removeImage(ImageItem *item);
    void cropRectSelected(CropRect *, QRectF);
    void cropRectChanged(QRectF);
    void saveCrop();
};

#endif // MAINWINDOW_H
