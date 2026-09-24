
#include "common/CrashHandler.h"
#include "common/Logger.h"
#include "core/Logger.h"
#include "core/PythonManager.h"
#include "core/QuickToolManager.h"
#include "core/Version.h"
#include "imgproc/ImgprocRegister.h"
#include "samples/SamplesRegister.h"
#include "deeplearning/DeepLearningRegister.h"

#include "quickui/Color.h"

#include <QApplication>
#include <QQmlApplicationEngine>

void qtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
    case QtDebugMsg:
        spdlog::debug("Qt: {}", localMsg.constData());
        break;
    case QtInfoMsg:
        spdlog::info("Qt: {}", localMsg.constData());
        break;
    case QtWarningMsg:
        spdlog::warn("Qt: {}", localMsg.constData());
        break;
    case QtCriticalMsg:
        spdlog::error("Qt: {}", localMsg.constData());
        break;
    case QtFatalMsg:
        spdlog::critical("Qt: {}", localMsg.constData());
        break;
    }
}

void initLog()
{
    try
    {
        auto logger = quicktools::common::setupLogger(quicktools::common::defaultSinks());
        logger->set_level(spdlog::level::debug);
        logger->set_pattern("[%Y/%m/%d %T.%e] [%n] [%^%L%$] [%t] %v");
        quicktools::core::registerLogger(logger);
        qInstallMessageHandler(qtMessageHandler);
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
    spdlog::info("欢迎使用 QuickTools! 版本: {}", quicktools::core::GetFullVersionString());

    //    QQuickStyle::setStyle("Basic");
    //    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");

    QApplication          app(argc, argv);
    quickui::QuiColor::getInstance()->setThemeMode(quickui::QuiColor::Light);

    QQmlApplicationEngine engine;
    engine.addImportPath(QCoreApplication::applicationDirPath());
    engine.addImportPath(QCoreApplication::applicationDirPath() + QStringLiteral("/qml"));
    engine.addImportPath(QCoreApplication::applicationDirPath() + QStringLiteral("/../qml"));

    quicktools::core::QuickToolManager::getInstance()->init();
    quicktools::imgproc::registerTools();
    quicktools::samples::registerTools();
    quicktools::dl::registerTools();
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
    spdlog::info("再见!");
    return exec;
}
