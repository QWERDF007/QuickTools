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

QuickToolManager::QuickToolManager(QObject *parent)
    : QObject(parent)
    , activated_tools(new ActivatedTools(this))
{
}

QuickToolManager *QuickToolManager::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    qmlEngine_ = qmlEngine;
    jsEngine_  = jsEngine;
    return getInstance();
}

bool QuickToolManager::addToActivted(AbstractQuickTool *tool)
{
    if (activated_tools == nullptr)
        return false;
    return activated_tools->addToActivated(tool);
}

bool QuickToolManager::removeFromActivated(AbstractQuickTool *tool)
{
    if (activated_tools == nullptr)
        return false;
    return activated_tools->removeFromActivated(tool);
}

AbstractQuickTool *QuickToolManager::createQuickTool(const int tool_type, QObject *parent)
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
            addToActivted(quick_tool);
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

int ActivatedTools::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(activated_tools_.size());
}

enum ActivatedQuickToolRoles
{
    NameRole = Qt::UserRole + 1,
    UuidRole,
};

QHash<int, QByteArray> ActivatedTools::roleNames() const
{
    return {
        {NameRole, "name"},
        {UuidRole, "uuid"},
    };
}

QVariant ActivatedTools::data(const QModelIndex &index, int role) const
{
    const int row = index.row();
    if (row < 0 || row >= rowCount())
        return QVariant();

    switch (role)
    {
    case NameRole:
        return activated_tools_.at(row)->name();
    case UuidRole:
        return activated_tools_.at(row)->uuid();
    default:
        return QVariant();
    }
}

bool ActivatedTools::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return QAbstractListModel::setData(index, value, role);
}

bool ActivatedTools::addToActivated(AbstractQuickTool *tool)
{
    if (tool == nullptr)
        return false;
    int row = static_cast<int>(activated_tools_.size());
    activated_tools_.push_back(tool);
    QModelIndex top_left = index(row, 0);
    insertRow(row);
    emit dataChanged(top_left, top_left, {});
    return true;
}

bool ActivatedTools::removeFromActivated(AbstractQuickTool *tool)
{
    if (tool == nullptr)
        return false;
    int size = static_cast<int>(activated_tools_.size());
    int offset{-1};
    for (int i = 0; i < size; ++i)
    {
        if (tool == activated_tools_.at(i))
        {
            offset = i;
            break;
        }
    }
    if (offset == -1)
        return false;
    activated_tools_.erase(activated_tools_.begin() + offset);
    removeRow(offset);
    return true;
}

} // namespace quicktools::core
