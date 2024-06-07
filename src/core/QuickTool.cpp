#include "core/QuickTool.h"

#include "common/Utils.h"
#include "priv/Predefined.h"

#include <chrono>
#include <sstream>

#undef slots
#include <pybind11/embed.h>
#define slots Q_SLOTS

namespace quicktools::core {

QuickToolFactor *QuickToolFactor::instance_  = nullptr;
QQmlEngine      *QuickToolFactor::qmlEngine_ = nullptr;
QJSEngine       *QuickToolFactor::jsEngine_  = nullptr;

AbstractQuickTool::AbstractQuickTool(QObject *parent)
    : QObject{parent}
    , helper_(new QuickToolHelper(this))
{
    // If auto-deletion is enabled, QThreadPool will automatically
    // delete this runnable after calling run();
    setAutoDelete(false);
}

AbstractQuickTool::~AbstractQuickTool()
{
    qDebug() << __FUNCTION__ << this;
}

int AbstractQuickTool::init()
{
    int ret = checkParams();
    if (ret == 0)
        ret = checkSettings();
    if (ret == 0)
        ret = doInInit();
    if (ret == 0)
        setIsInit(true);
    return ret;
}

int AbstractQuickTool::doInInit()
{
    return 0;
}

QString pythonErrorHandle(const pybind11::error_already_set &e)
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

void AbstractQuickTool::run()
{
    int     ret = 0;
    QString error_msg;
    if (!isInit())
        ret = init();
    if (ret != 0)
    {
        emit showMessage(InfoLevel::Error, tr("初始化失败"));
        return;
    }
    clearAlgorithmTime();
    emit started();
    auto start_time = std::chrono::high_resolution_clock::now();
    try
    {
        const auto &[status, msg] = process();

        ret       = status;
        error_msg = msg;
    }
    catch (const pybind11::error_already_set &e)
    {
        ret = -1;
        pybind11::gil_scoped_acquire acquire;
        error_msg = pythonErrorHandle(e);
    }
    catch (const std::exception &e)
    {
        ret       = -1;
        error_msg = e.what();
    }
    wall_clock_time_
        = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start_time).count();
    outputParams()->setToolTime(wall_clock_time_, algorithm_time_array_);
    outputParams()->setStatus(ret, error_msg);
    emit finished();
    emit showMessage(ret == 0 ? InfoLevel::Success : InfoLevel::Error, error_msg);
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

int AbstractQuickTool::initSettings()
{
    return 0;
}

bool AbstractQuickTool::isInit() const
{
    return is_init_;
}

void AbstractQuickTool::setIsInit(bool is_init)
{
    is_init_ = is_init;
}

int AbstractQuickTool::checkParams()
{
    int ret = checkInputParams();
    if (ret != 0)
    {
        return ret;
    }
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

void AbstractQuickTool::onRunAfterChanged()
{
    if (run_after_changed)
        submit();
}

void AbstractQuickTool::onSettingChanged(const QString &key, const QVariant &value)
{
    if (key == RUN_AFTER_CHANGED)
        run_after_changed = value.toBool();
}

/**
 * @note: 不能使用返回静态局部变量的指针, 否则结束时会报错 _CrtlsValidHeapPointer(block),
 *        应该是 Qt 对指针进行了 delete, 然后结束时静态变量又自己 delete, 导致 double delete.
 * @note: 此实现不是Meyers' Singleton! 可能存在问题?
 *        参考: https://www.zhihu.com/question/56527586/answer/2344903391
 */
QuickToolFactor *QuickToolFactor::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new QuickToolFactor;
    }
    return instance_;
}

QuickToolFactor *QuickToolFactor::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    qmlEngine_ = qmlEngine;
    jsEngine_  = jsEngine;
    return getInstance();
}

AbstractQuickTool *QuickToolFactor::createQuickTool(const int tool_type, QObject *parent) const
{
    auto found = tool_creators_.find(tool_type);
    if (found != tool_creators_.end())
    {
        auto               callable   = found->second;
        AbstractQuickTool *quick_tool = callable(parent);
        if (quick_tool)
        {
            quick_tool->init();
            quick_tool->setEngine(qmlEngine_, jsEngine_);
        }
        return quick_tool;
    }
    return nullptr;
}

void QuickToolFactor::registerQuickTool(const int tool_type, AbstractQuickToolCreator creator)
{
    auto found = tool_creators_.find(tool_type);
    assert(found == tool_creators_.end() && "This type is already registered");
    tool_creators_.emplace(tool_type, creator);
}

QString QuickToolFactor::groupUUID(const int group)
{
    auto found = groups_uuid_.find(group);
    if (found == groups_uuid_.end())
        return "";
    return found->second;
}

QString QuickToolFactor::taskUUID(const int task)
{
    auto found = tasks_uuid_.find(task);
    if (found == tasks_uuid_.end())
        return "";
    return found->second;
}

void QuickToolFactor::registerGroupAndTask(const int group, const int task)
{
    if (groups_uuid_.find(group) == groups_uuid_.end())
        groups_uuid_.emplace(group, common::uuid());
    if (tasks_uuid_.find(task) == tasks_uuid_.end())
        tasks_uuid_.emplace(task, common::uuid());
}

} // namespace quicktools::core
