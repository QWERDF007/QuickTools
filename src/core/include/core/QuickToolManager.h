#include "QuickTool.h"

namespace quicktools::core {

class ActivatedTools : public QAbstractListModel
{
public:
    ActivatedTools(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool     setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool addToActivated(AbstractQuickTool *tool);
    bool removeFromActivated(AbstractQuickTool *tool);

private:
    std::vector<AbstractQuickTool *> activated_tools_;
};

class QUICKTOOLS_CORE_EXPORT QuickToolManager : public QObject
{
    Q_OBJECT
    // 声明在 QML 本类为单例
    QML_SINGLETON
    // 声明在 QML 中通过 QuickToolManager 可访问本单例
    QML_NAMED_ELEMENT(QuickToolManager)
    Q_PROPERTY(ActivatedTools *activatedTools READ activatedTools CONSTANT FINAL)
public:
    using AbstractQuickToolCreator = std::function<AbstractQuickTool *(QObject *)>;

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

    ActivatedTools *activatedTools()
    {
        return activated_tools;
    }

    bool addToActivted(AbstractQuickTool *tool);

    bool removeFromActivated(AbstractQuickTool *tool);

    Q_INVOKABLE AbstractQuickTool *createQuickTool(const int type, QObject *parent = nullptr);

    void registerQuickTool(const int type, AbstractQuickToolCreator creator);

    Q_INVOKABLE QString getGroupUUID(const int group);
    Q_INVOKABLE QString getTaskUUID(const int task);
    void                registerGroupAndTask(const int group, const int task);

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

    ActivatedTools *activated_tools{nullptr};
};

} // namespace quicktools::core

#define REGISTER_QUICKTOOL(tool_type, ClassName)                                                            \
    inline ClassName *create##ClassName(QObject *parent = nullptr)                                          \
    {                                                                                                       \
        return new ClassName(parent);                                                                       \
    }                                                                                                       \
    inline void register##ClassName()                                                                       \
    {                                                                                                       \
        quicktools::core::QuickToolManager::getInstance()->registerQuickTool(tool_type, create##ClassName); \
    }
