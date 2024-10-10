#pragma once

#include "CoreGlobal.h"

#include <QAbstractListModel>
#include <QQmlPropertyMap>
#include <QtQml>

namespace quicktools::core {

namespace paramtypes {
Q_NAMESPACE

/**
 * @brief 参数类型枚举
 */
enum QuickToolParamType
{
    StatusParamType = 0,    //!< 状态
    TimeParamType,          //!< 运行时间
    TextParamType,          //!< 文本参数
    IntSpinBoxParamType,    //!< 整型旋钮
    DoubleSpinBoxParamType, //!< 浮点旋钮
    ComboBoxParamType,      //!< 下拉列表
    IntParamType = 2000,
    DoubleParamType,
    Int2DArrayParamType,
    Double2DArrayParamType,
    Text2DArrayParamType,
    InputFileParamType,         //!< 输入文件
    InputFolderParamType,       //!< 输入目录
    InputImageParamType = 4000, //!< 输入图像
    ShapesListParamType,        //!< 2D 形状
    InputAudioParamType = 6000, //!< 输入音频
};
Q_ENUM_NS(QuickToolParamType) // 向元对象系统注册枚举类型 QuickToolParamType，必须在 Q_NAMESPACE 宏声明的命名空间中
QML_NAMED_ELEMENT(QuickToolParamType) // 声明命名空间在 QML 中可用，以 QuickToolParam 进行访问

/**
 * @brief 参数数据的角色枚举
 */
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
    RunToolAfterParamChangedRole,
};

} // namespace paramtypes

class QUICKTOOLS_CORE_EXPORT AbstractQuickToolParams : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT FINAL) // FINAL 表明该属性不会被派生类覆盖
    Q_PROPERTY(QQmlPropertyMap *pdata READ pdata CONSTANT FINAL)
public:
    AbstractQuickToolParams(QObject *parent = nullptr, QQmlEngine * qml_engine = nullptr, QJSEngine* js_engine = nullptr);

    virtual ~AbstractQuickToolParams();

    virtual QString name() const = 0;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant data(const QString &name, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool setData(const QString &name, const QVariant &value);

    /**
     * @brief 添加参数
     * @param[in] name 参数名称
     * @param[in] display_name 参数展示名称
     * @param[in] desc 参数描述
     * @param[in] type 参数类型 @ref QuickToolParamType
     * @param[in] value 参数数据
     * @param[in] additional 参数附加数据
     * @param[in] editable 参数是否可编辑
     * @param[in] is_property 参数是否作为属性被界面访问
     * @param[in] run_tool_after_param_changed 是否在参数发生变化后运行工具
     * @param[in] visible 参数是否可见
     * @return
     */
    bool addParam(const QString &name, const QString &display_name, const QString &desc, const int type,
                  const QVariant &value, const QVariant &additional = QVariant(), const bool editable = false,
                  const bool is_property = false, const bool run_tool_after_param_changed = true,
                  const bool &visible = true);

    /**
     * @brief 属性指针, 用于界面直接访问参数的数据
     * @return
     */
    QQmlPropertyMap *pdata()
    {
        return &property_data_;
    }

    /**
     * @brief 获取参数的数量
     * @return
     */
    int size() const
    {
        return rowCount();
    }

    /**
     * @brief 获取参数是否为空
     * @return
     */
    bool empty() const
    {
        return size() <= 0;
    }

    /**
     * @brief 获取参数类型的名称
     * @param[in] type 参数类型 @ref QuickToolParamType
     * @return 参数类型的名称
     */
    static QString getTypeName(const int type);

    /**
     * @brief 获取参数是否初始化
     * @return
     */
    bool isInit() const
    {
        return is_init_;
    }

    /**
     * @brief 设置参数的初始化状态
     * @param is_init
     */
    void setIsInit(const bool is_init)
    {
        is_init_ = is_init;
    }

protected:
    /**
     * @brief 参数的名称
     */
    QList<QString> params_names_; // [name]

    /**
     * @brief 参数的数据, 每个参数包含多个数据 @ref QuickToolParamRole
     */
    QMap<QString, QMap<int, QVariant>> params_data_; // [name, [key, value]]

    /**
     * @brief 界面通过属性访问参数数据
     */
    QQmlPropertyMap property_data_; // QML 中可直接访问和修改对应 key 的属性

    QQmlEngine *qml_engine_{nullptr};
    QJSEngine  *js_engine_{nullptr};

private:
    bool setVisible(const QModelIndex &index, const QVariant &value);
    bool setValue(const QModelIndex &index, const QVariant &value);
    bool setImageValue(const QString & param_name, const QModelIndex &index, const QVariant &value);

    bool is_init_{false};

private slots:
    void onPropertyValueChanged(const QString &key, const QVariant &value);

signals:
    void runToolAfterParamChanged();
};

class QUICKTOOLS_CORE_EXPORT InputParams : public AbstractQuickToolParams
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(InputParams)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a InputParams directly")
public:
    InputParams(QObject *parent = nullptr, QQmlEngine * qml_engine = nullptr, QJSEngine* js_engine = nullptr)
        : AbstractQuickToolParams(parent, qml_engine, js_engine)
    {
    }

    virtual ~InputParams() {}

    QString name() const override
    {
        return "InputParams";
    }

    /**
     * @brief 添加下拉框参数
     * @param[in] name 参数名称
     * @param[in] display_name 参数展示名称
     * @param[in] desc 参数描述
     * @param[in] value 参数数据
     * @param[in] model 下拉列表模型
     * @param[in] is_property 参数是否作为属性被界面访问
     * @param[in] visible 参数是否可见
     * @return
     */
    bool addComboBox(const QString &name, const QString &display_name, const QString &desc, const QVariant &value,
                     const QVariantList &model, const bool is_property = false, const bool visible = true);

    /**
     * @brief 添加整型旋钮
     * @param[in] name 参数名称
     * @param[in] display_name 参数展示名称
     * @param[in] desc 参数描述
     * @param[in] value 参数数据
     * @param[in] from 参数下限
     * @param[in] to 参数上限
     * @param[in] is_property 参数是否作为属性被界面访问
     * @param[in] visible 参数是否可见
     * @return
     */
    bool addIntSpinBox(const QString &name, const QString &display_name, const QString &desc, const int value,
                       const int from = 0, const int to = 100, const bool is_property = false,
                       const bool visible = true);

    /**
     * @brief 添加浮点型旋钮
     * @param[in] name 参数名称
     * @param[in] display_name 参数展示名称
     * @param[in] desc 参数描述
     * @param[in] value 参数数据
     * @param[in] from 参数下限
     * @param[in] to 参数上限
     * @param[in] decimals 参数展示小数位数
     * @param[in] is_property 参数是否作为属性被界面访问
     * @param[in] visible 参数是否可见
     * @return
     */
    bool addDoubleSpinBox(const QString &name, const QString &display_name, const QString &desc, const double value,
                          const double from = 0, const double to = 1, const int decimals = 2,
                          const bool is_property = false, const bool visible = true);

    /**
     * @brief 添加输入图像参数
     * @param[in] name 参数名称
     * @param[in] display_name 参数展示名称
     * @param[in] desc 参数描述
     * @param[in] value 参数数据
     * @param[in] is_property 参数是否作为属性被界面访问
     * @param[in] visible 参数是否可见
     * @return
     */
    bool addImage(const QString &name, const QString &display_name, const QString &desc, const QVariant &value,
                       const bool is_property = false, const bool visible = true);
};

class QUICKTOOLS_CORE_EXPORT OutputParams : public AbstractQuickToolParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(OutputParams)
    QML_UNCREATABLE("Can't not create a AbstractOutputParams directly")
public:
    OutputParams(QObject *parent = nullptr, QQmlEngine * qml_engine = nullptr, QJSEngine* js_engine = nullptr);

    virtual ~OutputParams() {}

    QString name() const override
    {
        return "OutputParams";
    }

    /**
     * @brief 添加输出参数
     * @param[in] name 参数名称
     * @param[in] display_name 参数展示名称
     * @param[in] desc 参数描述
     * @param[in] type 参数类型 @ref QuickToolParamType
     * @param[in] value 参数数据
     * @param[in] additional 参数附加数据
     * @param[in] is_property 参数是否作为属性被界面访问
     * @param[in] visible 参数是否可见
     * @return
     */
    bool addParam(const QString &name, const QString &display_name, const QString &desc, const int type,
                  const QVariant &value, const QVariant &additional = QVariant(), const bool is_property = false,
                  const bool &visible = true);

    /**
     * @brief 设置工具运行时间
     * @param[in] wall_clock_time 工具整体运行时间
     * @param[in] algorithm_time 工具每个部分的运行时间
     * @return
     */
    bool setToolTime(const double wall_clock_time, const QList<double> &algorithm_time);

    /**
     * @brief 设置工具运行状态 @ref Error::Code
     * @param[in] status 工具运行状态
     * @param[in] msg 状态信息
     * @return
     */
    bool setStatus(const int status, const QString &msg);
};

} // namespace quicktools::core
