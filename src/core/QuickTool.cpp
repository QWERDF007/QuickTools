#include "core/QuickTool.h"

#include "common/Utils.h"
#include "core/QuickToolManager.h"
#include "priv/Predefined.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <sstream>

#undef slots
#include <pybind11/embed.h>
#define slots Q_SLOTS

namespace quicktools::core {

namespace {
QString pythonErrorHandle(const pybind11::error_already_set &e)
{
    try
    {
        auto traceback = pybind11::module::import("traceback").attr("format_exception");
        auto format_exception
            = traceback(e.type(), e.value() ? e.value() : pybind11::none(), e.trace() ? e.trace() : pybind11::none());
        std::stringstream sout;
        sout << e.what() << "\n\n";
        for (auto line : format_exception)
        {
            sout << line.cast<std::string>();
        }
        QString msg = QString::fromStdString(sout.str());
        return msg;
    }
    catch (...)
    {
        QString msg = "pythonErrorHandle Crash\n";
        msg += e.what();
        return msg;
    }
}
} // namespace

AbstractQuickTool::AbstractQuickTool(QObject *parent)
    : QObject{parent}
    , uuid_(common::uuid())
{
    // If auto-deletion is enabled, QThreadPool will automatically
    // delete this runnable after calling run();
    setAutoDelete(false);
}

AbstractQuickTool::~AbstractQuickTool()
{
    QuickToolManager::getInstance()->removeFromActivated(this);
}

std::tuple<int, QString> AbstractQuickTool::init()
{
    spdlog::info("初始化工具: {}, uuid: {}", name().toUtf8().constData(), uuid().toUtf8().constData());
    int     ret{0};
    QString msg{"初始化成功"};
    ret = checkParams();
    if (ret != 0)
        return {ret, "初始化输入/输出参数失败"};
    ret = checkSettings();
    if (ret != 0)
        return {ret, "初始化设置失败"};
    if (python_interface_)
    {
        const auto &[_ret, _msg] = python_interface_->init();

        ret = _ret;
        msg = _msg;
    }
    if (ret == 0)
        setIsInit(true);
    return {ret, msg};
}

void AbstractQuickTool::run()
{
    setRunning(true);
    if (!preprocess())
    {
        setRunning(false);
        return;
    }
    auto start_time  = std::chrono::high_resolution_clock::now();
    auto res         = process();
    auto end_time    = std::chrono::high_resolution_clock::now();
    wall_clock_time_ = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    postprocess(res);
    setRunning(false);
}

bool AbstractQuickTool::setProgress(const double v)
{
    if (v < 0 || v > 1 || v == progress_)
        return false;
    progress_ = v;
    emit progressChanged();
    return true;
}

void AbstractQuickTool::setEngine(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    qmlEngine_ = qmlEngine;
    jsEngine_  = jsEngine;
}

void AbstractQuickTool::submit()
{
    emit start();
    QThreadPool::globalInstance()->start(this);
}

int AbstractQuickTool::checkParams()
{
    int ret = checkInputParams();
    if (ret != 0)
        return ret;
    ret = checkOutputParams();
    return ret;
}

int AbstractQuickTool::checkInputParams()
{
    if (inputParams() == nullptr)
        return -1;
    if (!inputParams()->isInit())
    {
        int ret = initInputParams();
        if (ret == 0)
            inputParams()->setIsInit(true);
        return ret;
    }
    return 0;
}

int AbstractQuickTool::checkOutputParams()
{
    if (outputParams() == nullptr)
        return -1;
    if (!outputParams()->isInit())
    {
        int ret = initOutputParams();
        if (ret == 0)
            outputParams()->setIsInit(true);
        return ret;
    }
    return 0;
}

int AbstractQuickTool::checkSettings()
{
    if (settings() == nullptr)
        return -1;
    if (!settings()->isInit())
    {
        int ret = initSettings();
        if (ret == 0)
            settings()->setIsInit(true);
        return ret;
    }
    return 0;
}

bool AbstractQuickTool::preprocess()
{
    if (!isInit())
    {
        const auto &[ret, msg] = init();
        if (ret != 0)
        {
            emit showMessage(InfoLevel::Error, msg);
            spdlog::error(": {}, uuid: {}", msg.toUtf8().constData(), name().toUtf8().constData(),
                          uuid().toUtf8().constData());
            return false;
        }
    }
    clearAlgorithmTime();
    setProgress(0);
    emit started();
    return true;
}

std::tuple<int, QString> AbstractQuickTool::process()
{
    int     ret = 0;
    QString error_msg;
    try
    {
        const auto &[status, msg] = doInProcess();

        ret       = status;
        error_msg = msg;
    }
    catch (const pybind11::error_already_set &e)
    {
        ret       = -1;
        error_msg = e.what();
        //        pybind11::gil_scoped_acquire acquire;
        //        error_msg = pythonErrorHandle(e);
    }
    catch (const std::exception &e)
    {
        ret       = -1;
        error_msg = e.what();
    }
    catch (...)
    {
        ret       = -1;
        error_msg = tr("未知错误");
    }
    return {ret, error_msg};
}

void AbstractQuickTool::postprocess(const std::tuple<int, QString> &res)
{
    const auto &[ret, error_msg] = res;
    outputParams()->setToolTime(wall_clock_time_, algorithm_time_array_);
    outputParams()->setStatus(ret, error_msg);
    setProgress(1.0);
    emit finished();
    emit showMessage(ret == 0 ? InfoLevel::Success : InfoLevel::Error, error_msg);
    if (ret == 0)
        spdlog::info("运行成功: {}, uuid: {}", name().toUtf8().constData(), uuid().toUtf8().constData());
    else
        spdlog::error("运行失败: {}, uuid: {}, error: {}", name().toUtf8().constData(), uuid().toUtf8().constData(),
                      error_msg.toUtf8().constData());
}

void AbstractQuickTool::onRunAfterInputParamChanged()
{
    if (run_after_input_changed)
        submit();
}

void AbstractQuickTool::onSettingChange(const QString &key, const QVariant &value)
{
    if (key == Predefined::RUN_TOOL_AFTER_CHANGED)
        run_after_input_changed = value.toBool();
}

} // namespace quicktools::core
