
#include <QGuiApplication>
#include <QPalette>
#include <QQmlApplicationEngine>

#include "imgprocregister.h"

int main(int argc, char *argv[])
{
    QGuiApplication       app(argc, argv);
    QQmlApplicationEngine engine;

    quicktools::imgproc::registerTools();

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    qDebug() << "qml import path list" << engine.importPathList();
    engine.loadFromModule("QuickTools", "MainWindow");
    return app.exec();
}
