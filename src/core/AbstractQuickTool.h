#pragma once

#include "AbstractQuickToolParams.h"
#include "AbstractQuickToolSettings.h"
#include "CoreGlobal.h"
#include "QuickToolHelper.h"

#include <QObject>
#include <QString>
#include <QtQml>
#include <map>

namespace quicktools::core {



// Template classes not supported by Q_OBJECT

class QUICKTOOLS_CORE_EXPORT AbstractQuickTool
    : public QObject
    , public QRunnable
{
    Q_OBJECT

    // 声明 QML 中可用
    QML_NAMED_ELEMENT(QuickTool)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractQuickTool directly")

    Q_PROPERTY(AbstractInputParams *inputParams READ inputParams CONSTANT FINAL)
    Q_PROPERTY(AbstractOutputParams *outputParams READ outputParams CONSTANT FINAL)
    Q_PROPERTY(AbstractQuickToolSettings *settings READ settings CONSTANT FINAL)
    Q_PROPERTY(QuickToolHelper *helper READ helper CONSTANT FINAL)
    Q_PROPERTY(QString name READ name CONSTANT FINAL) // FINAL 表明该属性不会被派生类覆盖
    Q_PROPERTY(QString doc READ doc CONSTANT FINAL)
public:
    AbstractQuickTool(QObject *parent = nullptr);
    virtual ~AbstractQuickTool();

    virtual QString name() const
    {
        return "AbstractQuickTool";
    }

    virtual QString doc() const = 0;

    virtual std::tuple<int, QString> process() = 0;

    virtual int init();

    void run() override;

    virtual AbstractInputParams       *inputParams()  = 0;
    virtual AbstractOutputParams      *outputParams() = 0;
    virtual AbstractQuickToolSettings *settings()     = 0;

    /**
     * @brief 添加一个算法运行时间
     * @param algorithm_time
     */
    void addAlgorithmTime(const QVariant &algorithm_time)
    {
        algorithm_time_array_.append(algorithm_time);
    }

    /**
     * @brief 清除算法运行时间，执行算法之前被调用
     */
    void clearAlgorithmTime()
    {
        algorithm_time_array_.clear();
    }

    /**
     * @brief 绑定界面 engine，用于后续创建 ImageProvider
     * @param qmlEngine
     * @param jsEngine
     */
    void setEngine(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    QuickToolHelper *helper()
    {
        return helper_;
    }

    Q_INVOKABLE void submit();

    enum InfoLevel
    {
        Info,
        Warning,
        Error,
        Success,
        Custom
    };
    Q_ENUM(InfoLevel)

protected:
    virtual int initInputParams()  = 0;
    virtual int initOutputParams() = 0;
    virtual int initSettings();

    bool isInit() const;
    void setIsInit(bool);

protected slots:
    void onRunAfterChanged();
    void onSettingChanged(const QString &key, const QVariant &value);

private:
    Q_DISABLE_COPY(AbstractQuickTool)

    int checkParams();
    int checkInputParams();
    int checkOutputParams();
    int checkSettings();

    double       wall_clock_time_{0.};
    QVariantList algorithm_time_array_;

    bool run_after_changed{true};

    QQmlEngine *qmlEngine_{nullptr};
    QJSEngine  *jsEngine_{nullptr};

    QuickToolHelper *helper_{nullptr};

    bool is_init_{false};

signals:
    void start();
    void started();
    void finished();
    void showMessage(int, const QString &);
};

template<class InputParams, class OutputParams, class Settings>
class QUICKTOOLS_CORE_EXPORT AbstractTool : public AbstractQuickTool
{
    static_assert(std::is_base_of<AbstractInputParams, InputParams>::value,
                  "InputParams must be subclass of AbstractInputParams");
    static_assert(std::is_base_of<AbstractOutputParams, OutputParams>::value,
                  "OutputParams must be subclass of AbstractOutputParams");
    static_assert(std::is_base_of<AbstractQuickToolSettings, Settings>::value,
                  "Settings must be subclass of AbstractQuickToolSettings");

public:
    AbstractTool(QObject *parent = nullptr)
        : AbstractQuickTool(parent)
    {
        input_params_  = new InputParams(this);
        output_params_ = new OutputParams(this);
        settings_      = new Settings(this);
        connect(input_params_, &AbstractInputParams::runAfterChanged, this,
                &AbstractTool<InputParams, OutputParams, Settings>::onRunAfterChanged);
        connect(settings_, &AbstractQuickToolSettings::settingChanged, this,
                &AbstractTool<InputParams, OutputParams, Settings>::onSettingChanged);
    }

    virtual ~AbstractTool() {}

    AbstractInputParams *inputParams() override
    {
        return getInputParams();
    }

    InputParams *getInputParams()
    {
        return input_params_;
    }

    AbstractOutputParams *outputParams() override
    {
        return getOutputParams();
    }

    OutputParams *getOutputParams()
    {
        return output_params_;
    }

    AbstractQuickToolSettings *settings() override
    {
        return settings_;
    }

    Settings getSettings()
    {
        return settings_;
    }

protected:
    InputParams  *input_params_{nullptr};
    OutputParams *output_params_{nullptr};
    Settings     *settings_{nullptr};
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
        jsEngine_  = jsEngine;
        return getInstance();
    }

    Q_INVOKABLE AbstractQuickTool *createQuickTool(const int type, QObject *parent = nullptr) const;

    void registerQuickTool(const int type, AbstractQuickToolCreator creator);


    QString groupUUID(const int group);
    QString taskUUID(const int task);
    QString toolUUID(const int tool_type);
    void registerGroupAndTask(const int group, const int task);
    void registerQuickTool(const int group, const int task, const int tool_type, AbstractQuickToolCreator creator);

protected:
    static QQmlEngine *qmlEngine_;
    static QJSEngine  *jsEngine_;

private:
    /**
     * 禁用拷贝和移动构造和运算符
     */
    Q_DISABLE_COPY_MOVE(QuickToolFactor)

    explicit QuickToolFactor(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    /**
     * @brief 单例实例指针
     */
    static QuickToolFactor *instance_;

    std::map<int, AbstractQuickToolCreator> tool_creators_;

    std::map<int, std::map<int, std::map<int, AbstractQuickToolCreator>>> factors_;
    std::map<int, QString> groups_uuid_;
    std::map<int, QString> tasks_uuid_;
    std::map<int, QString> tools_uuid_;
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
