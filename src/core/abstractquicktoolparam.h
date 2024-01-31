#pragma once

#include "core_global.h"

#include <QAbstractListModel>
#include <QQmlPropertyMap>
#include <QtQml>

namespace quicktools::core {

namespace paramtypes {
Q_NAMESPACE

enum QuickToolParamType
{
    Text = 1,      //!< 文本参数
    IntSpinBox,    //!< 整型旋钮
    DoubleSpinBox, //!< 浮点旋钮
    ComboBox,      //!< 下拉列表
};
Q_ENUM_NS(QuickToolParamType) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中

enum QuickToolParamRole
{
    ParamNameRole = Qt::UserRole + 1,
    ParamTypeRole,
    ParamVisibleRole,
    ParamValueRole,
    ParamRangeRole,
    ParamIsPropertyRole,
    RunAfterParamChangedRole,
};
Q_ENUM_NS(QuickToolParamRole) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中

QML_NAMED_ELEMENT(QuickToolParam) // 声明封闭类型或命名空间在 QML 中可用，以 name 进行访问
} // namespace paramtypes

class QUICKTOOLS_CORE_EXPORT AbstractToolParams : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL) // FINAL 表明该属性不会被派生类覆盖
    Q_PROPERTY(QQmlPropertyMap *pdata READ pdata CONSTANT FINAL)
public:
    AbstractToolParams(QObject *parent = nullptr);

    virtual ~AbstractToolParams();

    virtual QString name() const = 0;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant data(const QString &name, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool setData(const QString &name, const QVariant &value);

    bool addParam(const QString &name, const int type, const bool is_property = false,
                  const bool run_after_changed = false, const QVariant &visible = true,
                  const QVariant &value = QVariant(), const QVariant &range = QVariant::fromValue(nullptr));

    QQmlPropertyMap *pdata()
    {
        return &property_data_;
    }

protected:
    QVector<QString>                   params_names_;
    QMap<QString, QMap<int, QVariant>> params_data_;
    QQmlPropertyMap                    property_data_; // QML 中可直接访问和修改对应 key 的属性

private slots:
    void onPropertyValueChanged(const QString &key, const QVariant &value);

signals:
    void nameChanged();
    void quicktoolRun();
};

class QUICKTOOLS_CORE_EXPORT AbstractInputParams : public AbstractToolParams
{
    Q_OBJECT
public:
    AbstractInputParams(QObject *parent = nullptr)
        : AbstractToolParams(parent)
    {
    }

    virtual ~AbstractInputParams() {}

    QString name() const override
    {
        return "AbstractInputParams";
    }
};

class QUICKTOOLS_CORE_EXPORT AbstractOutputParams : public AbstractToolParams
{
    Q_OBJECT
public:
    AbstractOutputParams(QObject *parent = nullptr)
        : AbstractToolParams(parent)
    {
    }

    virtual ~AbstractOutputParams() {}

    QString name() const override
    {
        return "AbstractOutputParams";
    }
};

} // namespace quicktools::core
