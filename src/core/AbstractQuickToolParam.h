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
    ParamTextType   = 1,    //!< 文本参数
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
    ParamIndexRole = Qt::UserRole + 1,
    ParamNameRole,
    ParamDisplayNameRole,
    ParamTypeRole,
    ParamTypeNameRole,
    ParamVisibleRole,
    ParamValueRole,
    ParamDisplayRole,
    ParamRangeRole,
    ParamIsPropertyRole,
    ParamEditableRole,
    RunAfterParamChangedRole,
};
Q_ENUM_NS(QuickToolParamRole) // 向元对象系统注册枚举类型 QuickToolParamRole，必须在 Q_NAMESPACE 宏声明的命名空间中

QML_NAMED_ELEMENT(QuickToolParam) // 声明命名空间在 QML 中可用，以 QuickToolParam 进行访问
} // namespace paramtypes

class QUICKTOOLS_CORE_EXPORT AbstractQuickToolParams : public QAbstractListModel
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(ToolParams)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractQuickToolParams directly")
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL) // FINAL 表明该属性不会被派生类覆盖
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

    bool addParam(const QString &en_name, const QString &zh_name, const int type, const QVariant &value,
                  const QVariant &range = QVariant(), const bool editable = false, const bool is_property = false,
                  const bool run_after_changed = true, const bool &visible = true);

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
    QVector<QString>                   params_names_;  // [name]
    QMap<QString, QMap<int, QVariant>> params_data_;   // [name, [key, value]]
    QQmlPropertyMap                    property_data_; // QML 中可直接访问和修改对应 key 的属性

private:
    bool is_init_{false};

private slots:
    void onPropertyValueChanged(const QString &key, const QVariant &value);

signals:
    void nameChanged();
    void runAfterChanged();
};

class QUICKTOOLS_CORE_EXPORT AbstractInputParams : public AbstractQuickToolParams
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(InputParams)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractInputParams directly")
public:
    AbstractInputParams(QObject *parent = nullptr)
        : AbstractQuickToolParams(parent)
    {
    }

    virtual ~AbstractInputParams() {}
};

class QUICKTOOLS_CORE_EXPORT AbstractOutputParams : public AbstractQuickToolParams
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(OutputParams)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractOutputParams directly")
public:
    AbstractOutputParams(QObject *parent = nullptr);

    virtual ~AbstractOutputParams() {}

    bool addParam(const QString &en_name, const QString &zh_name, const int type, const QVariant &value,
                  const QVariant &range = QVariant(), const bool is_property = false, const bool &visible = true);

    bool setToolTime(const double wall_clock_time, const QVariantList &algorithm_time);
    bool setStatus(const int status, const QString &msg);
};

} // namespace quicktools::core
