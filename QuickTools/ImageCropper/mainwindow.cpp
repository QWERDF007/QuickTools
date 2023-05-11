#include "mainwindow.h"

#include "imageitem.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QColor>
#include <QDebug>
#include <QElapsedTimer>
#include <QGraphicsOpacityEffect>
#include <QObject>
#include <QTabBar>
#include <QTime>
#include <QToolBox>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pool(QThreadPool::globalInstance())
{
    ui->setupUi(this);
    ui->toolBox->setStyleSheet("QToolBoxButton { height: 48px; /*font-size: 16px;*/}");

    connect(ui->actionOpenFolder, &QAction::triggered, this, &MainWindow::openFolder);
    connect(this, &MainWindow::newImage, this, &MainWindow::addImage);

    for (auto child : ui->tabWidget->findChildren<QTabBar *>())
    {
        child->hide();
    }

    for (auto child : ui->toolBox->children())
    {
        qDebug() << child << child->metaObject()->className();
    }
    ui->graphicsView->setScene(&scene_);
    qDebug() << pool->maxThreadCount();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readImage(const QFileInfoList &filelist)
{
    int    width              = ui->graphicsView->width();
    int    height             = ui->graphicsView->height();
    int    current_row_height = 0;
    int    row                = 0;
    int    col                = 0;
    QMutex mutex;
    i = 0;
    for (const auto &file : filelist) // Loop through list of files
    {
        QtConcurrent::run(pool,
                          [this, &mutex, &file, &row, &col, width, height, &current_row_height]()
                          {
                              qDebug() << file.fileName();
                              QPixmap pixmap(file.absoluteFilePath());
                              if (pixmap.isNull())
                              {
                                  return;
                              }
                              ImageItem *item
                                  = new ImageItem(pixmap); // Create QGraphicsPixmapItem object from QPixmap object

                              mutex.lock();
                              col += col == 0 ? 0 : 20;
                              int new_col = col + pixmap.width();
                              int new_row = row;
                              if (col && new_col > width)
                              {
                                  col = 0;
                                  row += current_row_height + 20;
                                  new_col            = pixmap.width();
                                  new_row            = row;
                                  current_row_height = pixmap.height();
                              }
                              else
                              {
                                  current_row_height = qMax(pixmap.height(), current_row_height);
                              }
                              item->setPos(col, row); // Set position of item
                              col = new_col;
                              row = new_row;
                              mutex.unlock();
                              emit newImage(item);
                          });
    }
    pool->waitForDone(); // Wait for all threads to finish
}

void MainWindow::openFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), "/home",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); // Open directory dialog
    if (folderPath.isEmpty())
    {
        return;
    }
    QDir        folder(folderPath);
    QStringList filters; // Create list of file filters
    filters << "*.png"
            << "*.jpg"
            << "*.jpeg"
            << "*.bmp"; // Add file filters to list
    QFileInfoList filelist
        = folder.entryInfoList(filters, QDir::Files); // Get list of files in directory matching filters
    readImage(filelist);
}

void MainWindow::addImage(ImageItem *item)
{
    int       min_edge  = qMin(item->pixmap().width(), item->pixmap().height());
    CropRect *crop_rect = new CropRect(0, 0, min_edge, min_edge, item);
    item->setCropRect(crop_rect);
    scene_.addItem(item); // Add QGraphicsPixmapItem object to scene_
    connect(crop_rect, &CropRect::rectClicked, this, &MainWindow::cropRectSelected);
    connect(crop_rect, &CropRect::rectChanged, this, &MainWindow::cropRectChanged);
}

void MainWindow::cropRectSelected(CropRect *crop_rect, QRectF rect)
{
    if (crop_rect)
    {
        QRect r = rect.toRect();
        ui->crop_rect_x->setValue(r.x());
        ui->crop_rect_y->setValue(r.y());
        ui->crop_rect_width->setValue(r.width());
        ui->crop_rect_height->setValue(r.height());
    }
}

void MainWindow::cropRectChanged(QRectF rect)
{
    if (!rect.isEmpty())
    {
        QRect r = rect.toRect();
        ui->crop_rect_x->setValue(r.x());
        ui->crop_rect_y->setValue(r.y());
        ui->crop_rect_width->setValue(r.width());
        ui->crop_rect_height->setValue(r.height());
    }
}
