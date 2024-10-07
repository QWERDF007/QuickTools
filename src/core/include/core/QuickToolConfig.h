#pragma once

#include "QuickToolManager.h"
#include "QuickToolType.h"

#include <QString>

namespace quicktools::core {

class AbstractQuickToolConfig
{
public:
    AbstractQuickToolConfig() {}

    virtual ~AbstractQuickToolConfig() {}

    QVariantMap toMap() const;

    virtual QString title() const = 0;

    virtual QString url() const = 0;

    virtual QString desc() const
    {
        return QString();
    }

    virtual QString icon() const
    {
        return QString();
    }

    virtual QVariantMap extra() const
    {
        return QVariantMap();
    }

    virtual int task() const
    {
        return quicktooltasktype::QuickToolTaskType::EmptyTask;
    }

    QString taskUUID() const
    {
        return QuickToolManager::getInstance()->getTaskUUID(task());
    }

    virtual int group() const
    {
        return quicktoolgrouptype::QuickToolGroupType::EmptyGroup;
    }

    QString groupUUID() const
    {
        return QuickToolManager::getInstance()->getGroupUUID(group());
    }
};

inline QVariantMap AbstractQuickToolConfig::toMap() const
{
    QVariantMap config;
    config["title"] = title();
    config["url"]   = url();
    config["desc"]  = desc();
    config["icon"]  = icon();
    config["extra"] = extra();
    config["task"]  = taskUUID();
    config["group"] = groupUUID();
    return config;
}

} // namespace quicktools::core
