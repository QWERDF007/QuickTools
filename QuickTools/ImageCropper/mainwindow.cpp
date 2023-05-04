#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "croprect.h"

#include <QDebug>
#include <QToolBox>
#include <QAction>
#include <QObject>
#include <QTabBar>
#include <QToolBox>
#include <QElapsedTimer>
#include <QTime>

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

    for(auto child : ui->tabWidget->findChildren<QTabBar *>())
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
    int width = ui->graphicsView->width();
    int height = ui->graphicsView->height();
    int current_row_height = 0;
    int row = 0;
    int col = 0;
    QMutex mutex;
    for (const auto &file : filelist) // Loop through list of files
    {
        QtConcurrent::run(pool, [this, &mutex, &file, &row, &col, width, height, &current_row_height]() {
            qDebug() << file.fileName();
            QPixmap pixmap(file.absoluteFilePath());
            if (pixmap.isNull())
            {
                return;
            }
            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap); // Create QGraphicsPixmapItem object from QPixmap object

            mutex.lock();
            col += col == 0 ? 0 : 20;
            int new_col = col + pixmap.width();
            int new_row = row;
            if (col && new_col > width)
            {
                col = 0;
                row += current_row_height + 20;
                new_col = pixmap.width();
                new_row = row;
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
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); // Open directory dialog
    if (folderPath.isEmpty())
    {
        return;
    }
    QDir folder(folderPath);
    QStringList filters; // Create list of file filters
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp"; // Add file filters to list
    QFileInfoList filelist = folder.entryInfoList(filters, QDir::Files); // Get list of files in directory matching filters
    readImage(filelist);
}

void MainWindow::addImage(QGraphicsPixmapItem *item)
{
    auto rect = new CropRect(0, 0, 200, 200, item);
    rect->setFlag(QGraphicsItem::ItemIsMovable, true);
    rect->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    scene_.addItem(item); // Add QGraphicsPixmapItem object to scene_
}
