#pragma once

#include "CoreGlobal.h"

#include <QAbstractListModel>
#include <QQmlPropertyMap>
#include <QtQml>

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractQuickToolSettings : public QAbstractListModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(QuickToolSettings)
    QML_UNCREATABLE("Can't not create a AbstractQuickToolSettings directly")
    Q_PROPERTY(QQmlPropertyMap *pdata READ pdata CONSTANT FINAL)
public:
    AbstractQuickToolSettings(QObject *parent = nullptr);

    virtual ~AbstractQuickToolSettings();

    QQmlPropertyMap *pdata()
    {
        return &property_data_;
    }

    virtual QString name() const = 0;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    enum SettingsRole
    {
        IndexRole = Qt::UserRole + 1,
        GroupRole,
        GroupNameRole,
        NameRole,
        DisplayNameRole,
        DescRole,
        TypeRole,
        VisibleRole,
        ValueRole,
        DisplayRole,
        AdditionalRole,
    };
    Q_ENUM(SettingsRole)

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    enum SettingsType
    {
        CheckBoxType = 0,
        SpinBoxType,
        DoubleSpinBoxType,
        ComboBoxType,
        ToggleSwitchType,
        SliderType,
        ColorDialogType,
        IntInputType,
    };
    Q_ENUM(SettingsType)

    enum SettingsGroup
    {
        BasicGroup = 0,
        DrawingGroup,
        UserGroup = 256,
    };

    int size() const
    {
        return rowCount();
    }

    bool empty() const
    {
        return size() <= 0;
    }

    bool isInit() const
    {
        return is_init_;
    }

    void setIsInit(const bool is_init)
    {
        is_init_ = is_init;
    }

    bool addGroup(const int group, const QString &group_name);

    bool addSetting(const int group, const QString &name, const QString &display_name, const QString &desc,
                    const int type, const QVariant &value, const QVariant &additional = QVariant(),
                    const bool &visible = true);

    QMap<QString, QMap<int, QVariant>> settings(const int group = -1) const;

    QString groupName(const int group) const;

    std::tuple<int, QString> copyFrom(AbstractQuickToolSettings *other, const int group = -1);

    bool addToogleSwitchSetting(const int group, const QString &name, const QString &display_name, const QString &desc,
                                const QVariant &value);
    bool addSliderSetting(const int group, const QString &name, const QString &display_name, const QString &desc,
                          const QVariant &value, const QVariant &from, const QVariant &to, const QVariant &step_size);
    bool addColorDialogSetting(const int group, const QString &name, const QString &display_name, const QString &desc,
                               const QVariant &value);
    bool addIntInputSetting(const int group, const QString &name, const QString &display_name, const QString &desc,
                            const QVariant &value, const QVariant &from = 0, const QVariant &to = 9999);

private:
    QList<QString>                     settings_names_; // [name]
    QMap<QString, QMap<int, QVariant>> settings_data_;  // [name, [key, value]]
    QQmlPropertyMap    property_data_; // [key, value] QML 中可直接访问和修改对应 key 的属性
    QMap<int, QString> groups_;        // [key, name]
    bool               is_init_{false};

private slots:
    void onPropertyValueChanged(const QString &key, const QVariant &value);

signals:
    void settingChanged(const QString &key, const QVariant &value);
};

class QUICKTOOLS_CORE_EXPORT QuickToolSettings : public AbstractQuickToolSettings
{
    Q_OBJECT
public:
    QuickToolSettings(QObject *parent = nullptr)
        : AbstractQuickToolSettings(parent)
    {
    }

    virtual ~QuickToolSettings() {}

    QString name() const override
    {
        return "QuickToolSettings";
    }
};

class QUICKTOOLS_CORE_EXPORT GlobalSettings : public AbstractQuickToolSettings
{
    Q_OBJECT
    QML_NAMED_ELEMENT(GlobalSettings)
    QML_UNCREATABLE("Can't not create a AbstractQuickToolSettings directly")
    QML_SINGLETON
public:
    QString name() const override
    {
        return "GlobalSettings";
    }

    /**
     * @brief 获取单例实例的指针
     * @return
     */
    static GlobalSettings *getInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new GlobalSettings;
        }
        return instance_;
    }

    /**
     * @brief 提供给 QML 创建一个单例实例的静态工厂函数
     * @param qmlEngine
     * @param jsEngine
     * @return
     */
    static GlobalSettings *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)
        return getInstance();
    }

private:
    explicit GlobalSettings(QObject *parent = nullptr);
    void addBasicSettings();

    static GlobalSettings *instance_;
};

} // namespace quicktools::core
