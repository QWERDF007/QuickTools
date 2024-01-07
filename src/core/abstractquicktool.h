#pragma once

#include "core_global.h"

#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QtQml>

namespace quicktools {

namespace quicktooltypes {
Q_NAMESPACE

enum QuickToolType
{
    ImageHistogram = 1, //!< 图像直方图
};
Q_ENUM_NS(QuickToolType) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中

enum QuickToolParamType
{
    Text = 1,      //!< 文本参数
    IntSpinBox,    //!< 整型旋钮
    DoubleSpinBox, //!< 浮点旋钮
    ComboBox,      //!< 下拉列表
};
Q_ENUM_NS(QuickToolParamType) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中

QML_NAMED_ELEMENT(QuickToolType) // 声明封闭类型或命名空间在 QML 中可用，以 name 进行访问
} // namespace quicktooltypes

class QUICKTOOLS_CORE_EXPORT AbstractToolParams : public QAbstractListModel
{
    Q_OBJECT
public:
    AbstractToolParams(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
    }

    virtual ~AbstractToolParams();

    enum ToolParamRole
    {
        ParamNameRole = Qt::UserRole + 1,
        ParamTypeRole,
        ParamVisibleRole,
        ParamValueRole,
        ParamRangeRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            {   ParamNameRole,    "name"},
            {   ParamTypeRole,    "type"},
            {ParamVisibleRole, "visible"},
            {  ParamValueRole,   "value"},
            {  ParamRangeRole,   "range"},
        };
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool addParam(const QString &name, const int type, const QVariant &value,
                  const QVariant &range = QVariant::fromValue(nullptr), const QVariant &visible = true);

protected:
    QVector<QMap<int, QVariant>> params_;
    //    QSet<QString> names_;
    //    QMap<QString, QMap<int, QVariant>> params_;
};

class QUICKTOOLS_CORE_EXPORT AbstractToolInputParams : public AbstractToolParams
{
    Q_OBJECT
public:
    AbstractToolInputParams(QObject *parent = nullptr)
        : AbstractToolParams(parent)
    {
    }

    virtual ~AbstractToolInputParams() {}
};

class QUICKTOOLS_CORE_EXPORT AbstractToolOutputParams : public AbstractToolParams
{
    Q_OBJECT
public:
    AbstractToolOutputParams(QObject *parent = nullptr)
        : AbstractToolParams(parent)
    {
    }

    virtual ~AbstractToolOutputParams() {}
};

class QUICKTOOLS_CORE_EXPORT AbstractQuickTool : public QObject
{
    Q_OBJECT

    // 声明 QML 中可用
    QML_ELEMENT
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractQuickTool directly")

    Q_PROPERTY(QAbstractListModel *inputParams READ inputParams NOTIFY inputParamsChanged FINAL)
    Q_PROPERTY(QAbstractListModel *outputParams READ outputParams NOTIFY outputParamsChanged FINAL)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL) // FINAL 表明该属性不会被派生类覆盖
    Q_PROPERTY(QUrl imageSource READ imageSource WRITE setImageSource NOTIFY imageSourceChanged FINAL)
public:
    AbstractQuickTool(QObject *parent = nullptr);
    virtual ~AbstractQuickTool();

    virtual QString name() const = 0;

    int operator()()
    {
        return run();
    }

    Q_INVOKABLE virtual int run() = 0;

    QAbstractListModel *inputParams() const
    {
        return input_params_;
    }

    bool setInputParams(AbstractToolInputParams *input_params);

    QAbstractListModel *outputParams() const
    {
        return output_params_;
    }

    bool setOutputParams(AbstractToolOutputParams *output_params);

    const QUrl &imageSource() const
    {
        return image_source_;
    }

    bool setImageSource(const QUrl &url);

protected:
    AbstractToolInputParams  *input_params_{nullptr};
    AbstractToolOutputParams *output_params_{nullptr};

    QUrl image_source_;

private:
    Q_DISABLE_COPY(AbstractQuickTool)

signals:
    void inputParamsChanged();
    void outputParamsChanged();
    void nameChanged();
    void imageSourceChanged();
};

class QUICKTOOLS_CORE_EXPORT QuickToolFactor : public QObject
{
    Q_OBJECT

    /**
     * 在 QML 中通过 QuickToolFactor 可访问本单例
     */
    QML_NAMED_ELEMENT(QuickToolFactor)

    /**
     * 在 QML 声明本类为单例
     */
    QML_SINGLETON

public:
    using AbstractQuickToolCreator = std::function<AbstractQuickTool *(void)>;

    /**
     * @brief 获取单例实例的指针
     * @return
     */
    static QuickToolFactor *getInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new QuickToolFactor;
        }
        return instance_;
    }

    /**
     * @brief 提供给 QML 创建一个单例实例的静态工厂函数
     * @param qmlEngine
     * @param jsEngine
     * @return
     */
    static QuickToolFactor *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)
        return getInstance();
    }

    Q_INVOKABLE AbstractQuickTool *createQuickTool(int type) const;

    void registerQuickTool(int type, AbstractQuickToolCreator creator);

private:
    explicit QuickToolFactor(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    /**
     * @brief 单例实例指针
     */
    static QuickToolFactor *instance_;

    std::map<int, AbstractQuickToolCreator> tool_creators_;

    /**
     * 禁用拷贝和移动构造和运算符
     */
    Q_DISABLE_COPY_MOVE(QuickToolFactor)
};

#define REGISTER_CLASS(tool_type, ClassName)                                             \
    inline ClassName *create##ClassName()                                                \
    {                                                                                    \
        return new ClassName;                                                            \
    }                                                                                    \
    inline void register##ClassName()                                                    \
    {                                                                                    \
        QuickToolFactor::getInstance()->registerQuickTool(tool_type, create##ClassName); \
    }

} // namespace quicktools
