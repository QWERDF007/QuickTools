
#include "common/CrashHandler.h"
#include "common/Logger.h"
#include "common/sqlite_sink.h"
#include "core/Logger.h"
#include "core/PythonManager.h"
#include "imgproc/ImgprocRegister.h"
#include "samples/SamplesRegister.h"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <QApplication>
#include <QQmlApplicationEngine>

void initLog()
{
    try
    {
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
        sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/log.txt", 23, 59));
        sinks.push_back(std::make_shared<spdlog::sinks::sqlite_sink_st>("db/log.db", 23, 59));
        auto logger = quicktools::common::setupLogger(sinks);
        logger->set_level(spdlog::level::debug);
        logger->set_pattern("[%Y/%m/%d %T.%e] [%n] [%^%L%$] [%t] %v");
        quicktools::core::registerLogger(logger);
    }
    catch (const std::exception &e)
    {
        qInfo() << __FUNCTION__ << e.what();
    }
}

int main(int argc, char *argv[])
{
    // 控制台stdout中文乱码: Edit -> Preferences -> Environment -> Interface -> Text codec for tools: UTF-8
    quicktools::common::CrashHandler crash_handler;
    crash_handler.setup();

    initLog();
    spdlog::info("欢迎使用 QuickTools!");

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
