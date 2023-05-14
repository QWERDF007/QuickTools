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
    , pool_(QThreadPool::globalInstance())
{
    ui->setupUi(this);
    ui->toolBox->setStyleSheet("QToolBoxButton { height: 48px; /*font-size: 16px;*/}");

    disableSpinBox();

    connect(ui->actionOpenFolder, &QAction::triggered, this, &MainWindow::openFolder);
    connect(this, &MainWindow::newImage, this, &MainWindow::addImage);
    connect(ui->image_view, &ImageView::removeImage, this, &MainWindow::removeImage);
    connect(ui->crop_rect_x, &QAbstractSpinBox::editingFinished, this, &MainWindow::rectEditingFinished);
    connect(ui->crop_rect_y, &QAbstractSpinBox::editingFinished, this, &MainWindow::rectEditingFinished);
    connect(ui->crop_rect_width, &QAbstractSpinBox::editingFinished, this, &MainWindow::rectEditingFinished);
    connect(ui->crop_rect_height, &QAbstractSpinBox::editingFinished, this, &MainWindow::rectEditingFinished);
    connect(ui->save_btn, &QAbstractButton::clicked, this, &MainWindow::saveCrop);

    ui->tabWidget->tabBar()->hide();

    for (auto child : ui->toolBox->children())
    {
        qDebug() << child << child->metaObject()->className();
    }
    ui->image_view->setScene(&scene_);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readImage(const QFileInfoList &filelist)
{
    int    width              = ui->image_view->width();
    int    current_row_height = 0;
    int    row                = 0;
    int    col                = 0;
    QMutex mutex;
    for (const auto &file : filelist)
    {
        QtConcurrent::run(pool_,
                          [this, &mutex, &file, &row, &col, width, &current_row_height]()
                          {
                              QPixmap pixmap(file.absoluteFilePath());
                              if (pixmap.isNull())
                              {
                                  return;
                              }
                              ImageItem *item = new ImageItem(pixmap);
                              mutex.lock();
                              image_fpaths_[item] = file.absoluteFilePath();
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
    pool_->waitForDone();
}

void MainWindow::setCropRectInfo(QRectF rect)
{
    QRect r = rect.toRect();
    ui->crop_rect_x->setValue(r.x());
    ui->crop_rect_y->setValue(r.y());
    ui->crop_rect_width->setValue(r.width());
    ui->crop_rect_height->setValue(r.height());
    int hcf = std::gcd(r.width(), r.height());
    ui->crop_rect_w_ratio->setValue(r.width() / hcf);
    ui->crop_rect_h_ratio->setValue(r.height() / hcf);
}

void MainWindow::setCropRect(QRect rect)
{
    if (selected_crop_rect_)
    {
        selected_crop_rect_->setRectFromParent(rect);
    }
}

void MainWindow::enableSpinBox()
{
    ui->crop_rect_x->setEnabled(true);
    ui->crop_rect_y->setEnabled(true);
    ui->crop_rect_width->setEnabled(true);
    ui->crop_rect_height->setEnabled(true);
    ui->crop_rect_w_ratio->setEnabled(true);
    ui->crop_rect_h_ratio->setEnabled(true);
}

void MainWindow::disableSpinBox()
{
    ui->crop_rect_x->setDisabled(true);
    ui->crop_rect_y->setDisabled(true);
    ui->crop_rect_width->setDisabled(true);
    ui->crop_rect_height->setDisabled(true);
    ui->crop_rect_w_ratio->setDisabled(true);
    ui->crop_rect_h_ratio->setDisabled(true);
}

void MainWindow::rectEditingFinished()
{
    int x      = ui->crop_rect_x->value();
    int y      = ui->crop_rect_y->value();
    int width  = ui->crop_rect_width->value();
    int height = ui->crop_rect_height->value();
    setCropRect(QRect(x, y, width, height));
}

void MainWindow::openFolder()
{
    // Open directory dialog
    QString folderPath = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (folderPath.isEmpty())
    {
        return;
    }
    QDir        folder(folderPath);
    QStringList filters;
    filters << "*.png"
            << "*.jpg"
            << "*.jpeg"
            << "*.bmp";
    // Get list of files in directory matching filters
    QFileInfoList filelist = folder.entryInfoList(filters, QDir::Files);
    readImage(filelist);
}

void MainWindow::addImage(ImageItem *item)
{
    int       min_edge  = qMin(item->pixmap().width(), item->pixmap().height());
    CropRect *crop_rect = new CropRect(0, 0, min_edge, min_edge, item);
    item->setCropRect(crop_rect);
    CloseButtonItem *btn = new CloseButtonItem(item);
    btn->setPos(item->pixmap().width() - 22, 2);
    scene_.addItem(item);
    connect(crop_rect, &CropRect::rectClicked, this, &MainWindow::cropRectSelected);
    connect(crop_rect, &CropRect::rectChanged, this, &MainWindow::cropRectChanged);
}

void MainWindow::removeImage(ImageItem *item)
{
    auto found = image_fpaths_.find(item);
    if (found != image_fpaths_.end())
    {
        image_fpaths_.remove(item);
    }
}

void MainWindow::cropRectSelected(CropRect *crop_rect, QRectF rect)
{
    if (crop_rect)
    {
        selected_crop_rect_ = crop_rect;
        setCropRectInfo(rect);
        enableSpinBox();
    }
}

void MainWindow::cropRectChanged(QRectF rect)
{
    if (!rect.isEmpty())
    {
        setCropRectInfo(rect);
    }
}

void MainWindow::saveCrop()
{
    QString folder_path = QFileDialog::getExistingDirectory(
        this, tr("Save"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (folder_path.isEmpty())
    {
        return;
    }
    QDir folder(folder_path);
    for (auto iter = image_fpaths_.cbegin(); iter != image_fpaths_.cend(); ++iter)
    {
        const ImageItem *item      = iter.key();
        QString          save_path = folder.absoluteFilePath(QFileInfo(iter.value()).fileName());

        QtConcurrent::run(pool_,
                          [item, save_path]()
                          {
                              QPixmap pm = item->cropped();
                              pm.save(save_path);
                          });
    }
}
