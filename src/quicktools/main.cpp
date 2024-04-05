
#include "imgprocregister.h"

#include <QApplication>

#include <QPalette>
#include <QQmlApplicationEngine>


int main(int argc, char *argv[])
{
    QApplication        app(argc, argv);
    QQmlApplicationEngine engine;

    quicktools::imgproc::registerTools();

    // QObject::connect(
    //     &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
    //     Qt::QueuedConnection);
    qDebug() << "qml import path list" << engine.importPathList();
    // engine.loadFromModule("QuickTools", "MainWindow");
    // return app.exec();
    const QUrl url(QStringLiteral("qrc:/qt/qml/QuickTools/qml/App.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);
    const int exec = QGuiApplication::exec();
    // if (exec == 931) {
    //     QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    // }
    return exec;
}
