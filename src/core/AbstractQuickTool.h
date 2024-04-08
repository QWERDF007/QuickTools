#pragma once

#include "AbstractQuickToolParam.h"
#include "CoreGlobal.h"

#include <QObject>
#include <QString>
#include <QtQml>
#include <map>

namespace quicktools::core {

namespace tooltypes {
Q_NAMESPACE

enum QuickToolType
{
    ImageHistogram = 1, //!< 图像直方图
};
Q_ENUM_NS(QuickToolType) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中

QML_NAMED_ELEMENT(QuickToolType) // 声明封闭类型或命名空间在 QML 中可用，以 name 进行访问
} // namespace tooltypes

// Template classes not supported by Q_OBJECT

class QUICKTOOLS_CORE_EXPORT AbstractQuickTool : public QObject
{
    Q_OBJECT

    // 声明 QML 中可用
    QML_NAMED_ELEMENT(QuickTool)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractQuickTool directly")

    Q_PROPERTY(AbstractInputParams *inputParams READ inputParams NOTIFY inputParamsChanged FINAL)
    Q_PROPERTY(AbstractOutputParams *outputParams READ outputParams NOTIFY outputParamsChanged FINAL)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL) // FINAL 表明该属性不会被派生类覆盖
public:
    AbstractQuickTool(QObject *parent = nullptr);
    virtual ~AbstractQuickTool();

    virtual QString name() const = 0;

    Q_INVOKABLE int exec();

    int init();

    virtual std::tuple<int, QString> run() = 0;

    AbstractInputParams *inputParams() const
    {
        return input_params_;
    }

    bool setInputParams(AbstractInputParams *input_params);

    AbstractOutputParams *outputParams() const
    {
        return output_params_;
    }

    bool setOutputParams(AbstractOutputParams *output_params);

    void setAlgorithmTime(const double algorithm_time)
    {
        algorithm_time_ = algorithm_time;
    }

    void setEngine(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

protected:
    virtual int initInputParams()  = 0;
    virtual int initOutputParams() = 0;

    int checkParams();
    int checkInputParams();
    int checkOutputParams();

    AbstractInputParams  *input_params_{nullptr};
    AbstractOutputParams *output_params_{nullptr};

    double  wall_clock_time_{0.};
    double  algorithm_time_{0.};
    int     status_{0};
    QString msg_;

    QQmlEngine *qmlEngine_{nullptr};
    QJSEngine *jsEngine_{nullptr};

private:
    Q_DISABLE_COPY(AbstractQuickTool)

signals:
    void inputParamsChanged();
    void outputParamsChanged();
    void nameChanged();
    void started();
    void finished();
};

template<class InputParams, class OutputParams>
class QUICKTOOLS_CORE_EXPORT AbstractTool : public AbstractQuickTool
{
    static_assert(std::is_base_of<AbstractInputParams, InputParams>::value,
                  "InputParams must be subclass of AbstractInputParams");
    static_assert(std::is_base_of<AbstractOutputParams, OutputParams>::value,
                  "OutputParams must be subclass of AbstractOutputParams");

public:
    AbstractTool(QObject *parent = nullptr)
        : AbstractQuickTool(parent)
    {
        input_params_  = new InputParams(this);
        output_params_ = new OutputParams(this);
        connect(input_params_, &AbstractInputParams::quicktoolStart, this, &AbstractQuickTool::exec);
    }

    virtual ~AbstractTool() {}
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
    using AbstractQuickToolCreator = std::function<AbstractQuickTool *(QObject *)>;

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
        qmlEngine_ = qmlEngine;
        jsEngine_ = jsEngine;
        return getInstance();
    }

    Q_INVOKABLE AbstractQuickTool *createQuickTool(int type, QObject *parent = nullptr) const;

    void registerQuickTool(int type, AbstractQuickToolCreator creator);

protected:
    static QQmlEngine *qmlEngine_;
    static QJSEngine *jsEngine_;

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

} // namespace quicktools::core

#define REGISTER_CLASS(tool_type, ClassName)                                                               \
    inline ClassName *create##ClassName(QObject *parent = nullptr)                                         \
    {                                                                                                      \
        return new ClassName(parent);                                                                      \
    }                                                                                                      \
    inline void register##ClassName()                                                                      \
    {                                                                                                      \
        quicktools::core::QuickToolFactor::getInstance()->registerQuickTool(tool_type, create##ClassName); \
    }
