
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

private:
    void readImage(const QFileInfoList &filelist);

    Ui::MainWindow *ui;

    QGraphicsScene scene_;

    QThreadPool *pool{nullptr};

    int i = 0;

signals:
    void newImage(ImageItem *);

private slots:
    void openFolder();
    void addImage(ImageItem *item);
    void cropRectSelected(CropRect *, QRectF);
    void cropRectChanged(QRectF);
};

#endif // MAINWINDOW_H
