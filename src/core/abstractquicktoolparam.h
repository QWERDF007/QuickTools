#pragma once

#include "core_global.h"

#include <QAbstractListModel>
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
};
Q_ENUM_NS(QuickToolParamRole) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中

QML_NAMED_ELEMENT(QuickToolParam) // 声明封闭类型或命名空间在 QML 中可用，以 name 进行访问
} // namespace paramtypes

class QUICKTOOLS_CORE_EXPORT AbstractToolParams : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL) // FINAL 表明该属性不会被派生类覆盖
public:
    AbstractToolParams(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
    }

    virtual ~AbstractToolParams();

    virtual QString name() const = 0;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool setData(const QString &name, const QVariant &value);

    bool addParam(const QString &name, const int type, const QVariant &value,
                  const QVariant &range = QVariant::fromValue(nullptr), const QVariant &visible = true);

protected:
    QVector<QString>                   params_names_;
    QMap<QString, QMap<int, QVariant>> params_data_;

signals:
    void nameChanged();
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
