#include "core/QuickTool.h"

#include "common/Utils.h"
#include "core/Error.h"
#include "core/PythonInterface.h"
#include "core/QuickToolManager.h"
#include "core/RuntimeParams.h"
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

AbstractQuickTool::AbstractQuickTool(QObject *parent, QQmlEngine *qml_engine, QJSEngine *js_engine)
    : QObject{parent}
    , qml_engine_(qml_engine)
    , js_engine_(js_engine)
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
    int     ret{Error::Success};
    QString msg;
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

bool AbstractQuickTool::initUI()
{
    int ret = initInputParams();
    if (ret == 0)
        inputParams()->setIsInit(true);
    ret = initOutputParams();
    if (ret == 0)
        outputParams()->setIsInit(true);
    ret = initSettings();
    if (ret == 0)
        settings()->setIsInit(true);
    return ret == Error::Success;
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

void AbstractQuickTool::submit()
{
    emit start();
    QThreadPool::globalInstance()->start(this);
    // TODO: 使用 std::thread 的线程池
    // Qt 线程池和 pytorch 一起使用有不知名的 BUG
    // 导入模块只能在主线程或者 std::thread, 否则会异常崩溃
    // 主线程导入 py 模块会稍微阻塞 UI 刷新
    // std::thread t([this]() { run(); });
    // t.detach();
}

void AbstractQuickTool::releasePythonModule()
{
    if (python_interface_ == nullptr)
        return;
    python_interface_->release();
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
    emit started();
    clearAlgorithmTime();
    setProgress(0);
    if (isInit())
        return true;
    const auto &[ret, msg] = init();
    if (ret == Error::Success)
        return true;
    emit showMessage(InfoLevel::Error, msg);
    spdlog::error(": {}, uuid: {}", msg.toUtf8().constData(), name().toUtf8().constData(), uuid().toUtf8().constData());
    return false;
}

std::tuple<int, QString> AbstractQuickTool::process()
{
    int     ret = 0;
    QString error_msg;
    try
    {
        if (runtime_params_)
        {
            runtime_params_->reset();
            bool ok = runtime_params_->getInput(inputParams());
            if (!ok)
                return runtime_params_->returnError();
        }
        const auto &[status, msg] = doInProcess();
        if (runtime_params_)
        {
            bool ok = runtime_params_->genOutput(outputParams());
            if (!ok)
                return runtime_params_->returnError();
        }
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

void quicktools::core::AbstractQuickTool::reloadModule()
{
    if (python_interface_)
    {
        const auto &[ret, msg] = python_interface_->reloadModule();
        emit showMessage(ret == 0 ? InfoLevel::Info : InfoLevel::Error, msg);
    }
}

} // namespace quicktools::core
