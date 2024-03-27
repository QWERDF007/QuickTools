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
    ParamStatusType = 0,
    ParamTextType = 1,      //!< 文本参数
    ParamIntSpinBoxType,    //!< 整型旋钮
    ParamDoubleSpinBoxType, //!< 浮点旋钮
    ParamComboBoxType,      //!< 下拉列表
    ParamIntType = 2000,
    ParamDoubleType,
    ParamInt1DArrayType,
    ParamDouble1DArrayType,
    ParamText1DArrayType,
    ParamInt2DArrayType,
    ParamDouble2DArrayType,
    ParamText2DArrayType,
    ParamImageType = 4000, //!< 图像
    ParamAudioType = 6000, //!< 音频
};
Q_ENUM_NS(QuickToolParamType) // 向元对象系统注册枚举类型 QuickToolParamType，必须在 Q_NAMESPACE 宏声明的命名空间中

enum QuickToolParamRole
{
    ParamNameRole = Qt::UserRole + 1,
    ParamTypeRole,
    ParamTypeNameRole,
    ParamVisibleRole,
    ParamValueRole,
    ParamRangeRole,
    ParamIsPropertyRole,
    ParamEditableRole,
    RunAfterParamChangedRole,
};
Q_ENUM_NS(QuickToolParamRole) // 向元对象系统注册枚举类型 QuickToolParamRole，必须在 Q_NAMESPACE 宏声明的命名空间中

QML_NAMED_ELEMENT(QuickToolParam) // 声明命名空间在 QML 中可用，以 QuickToolParam 进行访问
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

    bool addParam(const QString &en_name, const QString& zh_name, const int type, const bool is_property, const bool run_after_changed,
                  const bool editable = true, const QVariant &visible = true, const QVariant &value = QVariant(),
                  const QVariant &range = QVariant());

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

protected:
    QVector<QString>                   params_names_;
    QMap<QString, QMap<int, QVariant>> params_data_;
    QQmlPropertyMap                    property_data_; // QML 中可直接访问和修改对应 key 的属性

private slots:
    void onPropertyValueChanged(const QString &key, const QVariant &value);

signals:
    void nameChanged();
    void quicktoolStart();
};

class QUICKTOOLS_CORE_EXPORT AbstractInputParams : public AbstractToolParams
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(InputParams)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractInputParams directly")
public:
    AbstractInputParams(QObject *parent = nullptr)
        : AbstractToolParams(parent)
    {
    }

    virtual ~AbstractInputParams() {}
};

class QUICKTOOLS_CORE_EXPORT AbstractOutputParams : public AbstractToolParams
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(OutputParams)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractOutputParams directly")
public:
    AbstractOutputParams(QObject *parent = nullptr);

    virtual ~AbstractOutputParams() {}

    bool addParam(const QString &en_name, const QString &zh_name, const int type, const bool is_property, const QVariant &visible = true,
                  const QVariant &value = QVariant(), const QVariant &range = QVariant());

    bool setToolTime(const double wall_clock_time, const double algorithm_time);
    bool setStatus(const int status, const QString& msg);
};

} // namespace quicktools::core
