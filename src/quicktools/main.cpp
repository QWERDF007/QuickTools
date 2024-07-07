
#include "common/CrashHandler.h"
#include "imgproc/ImgprocRegister.h"
#include "core/PythonManager.h"
#include "samples/SamplesRegister.h"

#include <QApplication>
#include <QQmlApplicationEngine>

#include <spdlog/spdlog.h>

void initLog()
{
    // Set global log level to debug
    spdlog::set_level(spdlog::level::debug);
    // change log pattern
    spdlog::set_pattern("[%Y%m%d %H:%M:%S.%e] [%n] [%L] [%t] %v");
}

int main(int argc, char *argv[])
{
    quicktools::common::CrashHandler crash_handler;
    crash_handler.setup();

    initLog();
    spdlog::info("Welcome to QuickTools!");

//    QQuickStyle::setStyle("Basic");
//    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");

    QApplication          app(argc, argv);
    QQmlApplicationEngine engine;

    quicktools::imgproc::registerTools();
    quicktools::samples::registerTools();
    quicktools::core::PythonManager::getInstance()->init();

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
