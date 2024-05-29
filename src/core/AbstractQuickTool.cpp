#include "AbstractQuickTool.h"

#include "Utils.h"
#include "priv/Predefined.h"

#include <chrono>

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

void AbstractQuickTool::run()
{
    int ret = 0;
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
        auto end_time             = std::chrono::high_resolution_clock::now();
        wall_clock_time_          = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        outputParams()->setToolTime(wall_clock_time_, algorithm_time_array_);
        outputParams()->setStatus(status, msg);
        emit finished();
        emit showMessage(status == 0 ? InfoLevel::Success : InfoLevel::Error, msg);
    }
    catch (const std::exception &e)
    {
        auto end_time    = std::chrono::high_resolution_clock::now();
        wall_clock_time_ = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        outputParams()->setToolTime(wall_clock_time_, algorithm_time_array_);
        outputParams()->setStatus(-1, e.what());
        emit finished();
        emit showMessage(InfoLevel::Error, e.what());
    }
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
