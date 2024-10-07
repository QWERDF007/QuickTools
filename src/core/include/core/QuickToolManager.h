#pragma once

#include "QuickTool.h"
#include "QuickToolType.h"

#include <QAbstractTableModel>

namespace quicktools::core {

class AbstractQuickToolConfig;

class ActivatedTools : public QAbstractTableModel
{
public:
    ActivatedTools(QObject *parent = nullptr)
        : QAbstractTableModel(parent)
    {
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool     setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool addToActivated(AbstractQuickTool *tool);
    bool removeFromActivated(AbstractQuickTool *tool);

    const std::vector<AbstractQuickTool *> getActivatedTools() const;

private:
    void onToolRunningChanged();

    std::vector<AbstractQuickTool *> activated_tools_;
};

inline const std::vector<AbstractQuickTool *> ActivatedTools::getActivatedTools() const
{
    return activated_tools_;
}

class QUICKTOOLS_CORE_EXPORT QuickToolManager : public QObject
{
    Q_OBJECT
    // 声明在 QML 本类为单例
    QML_SINGLETON
    // 声明在 QML 中通过 QuickToolManager 可访问本单例
    QML_NAMED_ELEMENT(QuickToolManager)
    Q_PROPERTY(ActivatedTools *activatedTools READ activatedTools CONSTANT FINAL)
public:
    using AbstractQuickToolCreator       = std::function<AbstractQuickTool *(QObject *)>;
    using AbstractQuickToolConfigCreator = std::function<AbstractQuickToolConfig *(void)>;

    /**
     * @brief 获取单例实例的指针
     * @return
     */
    static QuickToolManager *getInstance();

    /**
     * @brief 提供给 QML 创建一个单例实例的静态工厂函数
     * @param qmlEngine
     * @param jsEngine
     * @return
     */
    static QuickToolManager *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    int init();

    ActivatedTools *activatedTools()
    {
        return activated_tools_;
    }

    bool addToActivted(AbstractQuickTool *tool);

    bool removeFromActivated(AbstractQuickTool *tool);

    Q_INVOKABLE AbstractQuickTool *createQuickTool(const int type, QObject *parent = nullptr);

    void registerQuickTool(const int type, AbstractQuickToolCreator tool_creator,
                           AbstractQuickToolConfigCreator config_creator);

    Q_INVOKABLE QString getGroupUUID(const int group);
    Q_INVOKABLE QString getTaskUUID(const int task);
    void                registerGroupAndTask(const int group, const int task);

    Q_INVOKABLE QList<QVariantMap> getToolsConfig(const QString &group_uuid, const QString &task_uuid) const;
    Q_INVOKABLE QList<QVariantMap> getRecentlyAddedToolsConfig() const;
    Q_INVOKABLE QList<QVariantMap> getRecentlyUpdatedToolsConfig() const;
    Q_INVOKABLE int                getRecentlyChangedToolsCount();

protected:
    static QQmlEngine *qmlEngine_;
    static QJSEngine  *jsEngine_;

private:
    Q_DISABLE_COPY_MOVE(QuickToolManager)

    explicit QuickToolManager(QObject *parent = nullptr);

    /**
     * @brief 单例实例指针
     */
    static QuickToolManager *instance_;

    std::map<int, AbstractQuickToolCreator> tool_creators_;

    std::map<int, QString> groups_uuid_;
    std::map<int, QString> tasks_uuid_;

    std::map<QString, std::map<QString, std::map<int, QVariantMap>>> tools_config_; // group, task, tool

    std::map<int, QVariantMap> recently_updated_tools_config_;
    std::map<int, QVariantMap> recently_added_tools_config_;

    ActivatedTools *activated_tools_{nullptr};
};

} // namespace quicktools::core

#define REGISTER_QUICKTOOL(tool_type, ClassName, ConfigClassName)                                          \
    inline ClassName *create##ClassName(QObject *parent = nullptr)                                         \
    {                                                                                                      \
        return new ClassName(parent);                                                                      \
    }                                                                                                      \
    inline ConfigClassName *create##ConfigClassName()                                                      \
    {                                                                                                      \
        return new ConfigClassName();                                                                      \
    }                                                                                                      \
    inline void register##ClassName()                                                                      \
    {                                                                                                      \
        quicktools::core::QuickToolManager::getInstance()->registerQuickTool(tool_type, create##ClassName, \
                                                                             create##ConfigClassName);     \
    }
