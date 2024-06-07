#pragma once

#include "CoreGlobal.h"

#include <QAbstractListModel>
#include <QQmlPropertyMap>
#include <QtQml>

namespace quicktools::core {

namespace paramtypes {
Q_NAMESPACE

enum QuickToolParamType
{
    StatusParamType = 0,
    TimeParamType,
    TextParamType,          //!< 文本参数
    IntSpinBoxParamType,    //!< 整型旋钮
    DoubleSpinBoxParamType, //!< 浮点旋钮
    ComboBoxParamType,      //!< 下拉列表
    IntParamType = 2000,
    DoubleParamType,
    Int1DArrayParamType,
    Double1DArrayParamType,
    Text1DArrayParamType,
    Int2DArrayParamType,
    Double2DArrayParamType,
    Text2DArrayParamType,
    ImageParamType = 4000, //!< 图像
    AudioParamType = 6000, //!< 音频
};
Q_ENUM_NS(QuickToolParamType) // 向元对象系统注册枚举类型 QuickToolParamType，必须在 Q_NAMESPACE 宏声明的命名空间中
QML_NAMED_ELEMENT(QuickToolParamType) // 声明命名空间在 QML 中可用，以 QuickToolParam 进行访问

enum QuickToolParamRole
{
    ParamIndexRole = Qt::UserRole + 1,
    ParamNameRole,
    ParamDisplayNameRole,
    ParamDescRole,
    ParamTypeRole,
    ParamTypeNameRole,
    ParamVisibleRole,
    ParamValueRole,
    ParamDisplayRole,
    ParamAdditionalRole,
    ParamIsPropertyRole,
    ParamEditableRole,
    RunAfterParamChangedRole,
};

} // namespace paramtypes

class QUICKTOOLS_CORE_EXPORT AbstractQuickToolParams : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT FINAL) // FINAL 表明该属性不会被派生类覆盖
    Q_PROPERTY(QQmlPropertyMap *pdata READ pdata CONSTANT FINAL)
public:
    AbstractQuickToolParams(QObject *parent = nullptr);

    virtual ~AbstractQuickToolParams();

    virtual QString name() const = 0;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant data(const QString &name, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool setData(const QString &name, const QVariant &value);

    bool addParam(const QString &name, const QString &display_name, const QString &desc, const int type,
                  const QVariant &value, const QVariant &additional = QVariant(), const bool editable = false,
                  const bool is_property = false, const bool run_after_changed = true, const bool &visible = true);

    QQmlPropertyMap *pdata()
    {
        return &property_data_;
    }

    int size() const
    {
        return rowCount();
    }

    bool empty() const
    {
        return size() <= 0;
    }

    static QString getTypeName(const int type);

    bool isInit() const
    {
        return is_init_;
    }

    void setIsInit(const bool is_init)
    {
        is_init_ = is_init;
    }

protected:
    QList<QString>                     params_names_;  // [name]
    QMap<QString, QMap<int, QVariant>> params_data_;   // [name, [key, value]]
    QQmlPropertyMap                    property_data_; // QML 中可直接访问和修改对应 key 的属性

private:
    bool is_init_{false};

private slots:
    void onPropertyValueChanged(const QString &key, const QVariant &value);

signals:
    void runAfterChanged();
};

class QUICKTOOLS_CORE_EXPORT InputParams : public AbstractQuickToolParams
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(InputParams)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a InputParams directly")
public:
    InputParams(QObject *parent = nullptr)
        : AbstractQuickToolParams(parent)
    {
    }

    virtual ~InputParams() {}

    QString name() const override
    {
        return "InputParams";
    }
};

class QUICKTOOLS_CORE_EXPORT OutputParams : public AbstractQuickToolParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(OutputParams)
    QML_UNCREATABLE("Can't not create a AbstractOutputParams directly")
public:
    OutputParams(QObject *parent = nullptr);

    virtual ~OutputParams() {}

    QString name() const override
    {
        return "OutputParams";
    }

    bool addParam(const QString &name, const QString &display_name, const QString &desc, const int type,
                  const QVariant &value, const QVariant &additional = QVariant(), const bool is_property = false,
                  const bool &visible = true);

    bool setToolTime(const double wall_clock_time, const QList<double> &algorithm_time);
    bool setStatus(const int status, const QString &msg);
};

} // namespace quicktools::core
