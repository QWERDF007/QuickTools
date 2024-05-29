
#include "CrashHandler.h"
#include "imgprocregister.h"

#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    quicktools::common::CrashHandler crash_handler;
    crash_handler.setup();

//    QQuickStyle::setStyle("Basic");
//    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");

    QApplication          app(argc, argv);
    QQmlApplicationEngine engine;

    quicktools::imgproc::registerTools();

    qDebug() << "qml import path list" << engine.importPathList();
    const QUrl url(QStringLiteral("qrc:/qt/qml/QuickTools/App.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);
    const int exec = QGuiApplication::exec();
    // if (exec == 931) {
    //     QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    // }

    return exec;
}
