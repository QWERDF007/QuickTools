#include "core/QuickToolManager.h"

#include "common/Utils.h"

#include <spdlog/spdlog.h>


namespace quicktools::core {

QuickToolManager *QuickToolManager::instance_  = nullptr;
QQmlEngine       *QuickToolManager::qmlEngine_ = nullptr;
QJSEngine        *QuickToolManager::jsEngine_  = nullptr;

/**
 * @note: 不能使用返回静态局部变量的指针, 否则结束时会报错 _CrtlsValidHeapPointer(block),
 *        应该是 Qt 对指针进行了 delete, 然后结束时静态变量又自己 delete, 导致 double delete.
 * @note: 此实现不是Meyers' Singleton! 可能存在问题?
 *        参考: https://www.zhihu.com/question/56527586/answer/2344903391
 */
QuickToolManager *QuickToolManager::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new QuickToolManager;
    }
    return instance_;
}

QuickToolManager *QuickToolManager::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    qmlEngine_ = qmlEngine;
    jsEngine_  = jsEngine;
    return getInstance();
}

AbstractQuickTool *QuickToolManager::createQuickTool(const int tool_type, QObject *parent) const
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
            spdlog::info("创建工具: {}, uuid: {}", quick_tool->name().toUtf8().constData(),
                         quick_tool->uuid().toUtf8().constData());
            return quick_tool;
        }
    }
    spdlog::error("创建工具失败, type: {}", tool_type);
    return nullptr;
}

void QuickToolManager::registerQuickTool(const int tool_type, AbstractQuickToolCreator creator)
{
    auto found = tool_creators_.find(tool_type);
    assert(found == tool_creators_.end() && "This type is already registered");
    tool_creators_.emplace(tool_type, creator);
}

QString QuickToolManager::getGroupUUID(const int group)
{
    auto found = groups_uuid_.find(group);
    if (found == groups_uuid_.end())
        return "";
    return found->second;
}

QString QuickToolManager::getTaskUUID(const int task)
{
    auto found = tasks_uuid_.find(task);
    if (found == tasks_uuid_.end())
        return "";
    return found->second;
}

void QuickToolManager::registerGroupAndTask(const int group, const int task)
{
    if (groups_uuid_.find(group) == groups_uuid_.end())
        groups_uuid_.emplace(group, common::uuid());
    if (tasks_uuid_.find(task) == tasks_uuid_.end())
        tasks_uuid_.emplace(task, common::uuid());
}

} // namespace quicktools::core
