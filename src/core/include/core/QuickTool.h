#pragma once

#include "CoreGlobal.h"
#include "QuickToolHelper.h"
#include "QuickToolParams.h"
#include "QuickToolSettings.h"

#include <QObject>
#include <QString>
#include <QtQml>

namespace quicktools::core {

// Template classes not supported by Q_OBJECT

/**
 * @brief QuickTools 抽象接口, 定义了界面可访问的属性、方法
 */
class QUICKTOOLS_CORE_EXPORT AbstractQuickTool
    : public QObject
    , public QRunnable
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(QuickTool)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractQuickTool directly")

    /// 工具输入参数
    Q_PROPERTY(InputParams *inputParams READ inputParams CONSTANT FINAL)
    /// 工具输出参数
    Q_PROPERTY(OutputParams *outputParams READ outputParams CONSTANT FINAL)
    /// 工具设置项
    Q_PROPERTY(AbstractQuickToolSettings *settings READ settings CONSTANT FINAL)
    Q_PROPERTY(QuickToolHelper *helper READ helper CONSTANT FINAL)
    /// 工具名称
    Q_PROPERTY(QString name READ name CONSTANT FINAL) // FINAL 表明该属性不会被派生类覆盖
    /// 工具文档
    Q_PROPERTY(QString doc READ doc CONSTANT FINAL)
    /// 工具运行进度
    Q_PROPERTY(double progress READ progress WRITE setProgress NOTIFY progressChanged)
    /// 工具实例唯一标识
    Q_PROPERTY(QString uuid READ uuid CONSTANT FINAL)
    /// 工具是否包含 python
    Q_PROPERTY(bool hasPython READ hasPython CONSTANT FINAL)
public:
    AbstractQuickTool(QObject *parent = nullptr);
    virtual ~AbstractQuickTool();

    /**
     * @brief 获取工具名称
     * @return 工具名称
     */
    virtual QString name() const
    {
        return "AbstractQuickTool";
    }

    /**
     * @brief 获取工具说明文档
     * @return 工具说明文档
     */
    virtual QString doc() const = 0;

    /**
     * @brief 初始化工具, 检查输入、输出参数是否初始化, 检查设置项是否初始化, 执行 @ref doInInit, 设置初始化状态
     * @return 初始化结果状态码
     * @retval 0 初始化成功
     */
    int init();

    /**
     * @brief 获取工具实例的唯一识别码
     * @return 工具实例的唯一识别码
     * @see common::uuid()
     */
    QString uuid() const
    {
        return uuid_;
    }

    /**
     * @brief 运行工具, 依次调用 @ref preprocess, @ref process, @ref postprocess
     */
    void run() override;

    /**
     * @brief 初始化输入参数, 由子类实现, 决定如何初始化
     * @return
     */
    virtual InputParams *inputParams() = 0;

    /**
     * @brief 初始化输出参数, 由子类实现, 决定如何初始化
     * @return
     */
    virtual OutputParams *outputParams() = 0;

    /**
     * @brief 获取工具的设置选项
     * @return
     */
    virtual AbstractQuickToolSettings *settings() = 0;

    /**
     * @brief 获取工具的运行进度
     * @return [0,1]
     */
    double progress() const
    {
        return progress_;
    }

    /**
     * @brief 设置运行进度条
     * @param [in] 运行进度
     * @return
     */
    bool setProgress(const double v);

    /**
     * @brief 添加一个算法运行时间 (ms)
     * @param [in] algorithm_time 算法时间
     */
    void addAlgorithmTime(const double algorithm_time)
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
     * @param [in] qmlEngine
     * @param [in] jsEngine
     */
    void setEngine(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    QuickToolHelper *helper()
    {
        return helper_;
    }

    /**
     * @brief 工具是否包含 python, 默认 false, 具有 python 的工具重写此函数
     * @return
     */
    virtual bool hasPython() const
    {
        return false;
    }

    /**
     * @brief 向 Qt 的线程池中提交本工具的运行任务
     */
    Q_INVOKABLE void submit();

    /*
     * @brief 通知级别
     */
    enum InfoLevel
    {
        Info    = 0, //!< 通知
        Warning = 1, //!< 警告
        Error   = 2, //!< 错误
        Success = 3, //!< 成功
        Custom  = 4, //!< 自定义
    };
    Q_ENUM(InfoLevel)

    /**
     * @brief 获取工具是否正在运行
     * @return
     */
    bool isRunning() const
    {
        return running_;
    }

    /**
     * @brief 获取工具初始化的状态
     * @return 工具初始化的状态
     */
    bool isInit() const;

    /**
     * @brief 设置工具初始化的状态
     */
    void setIsInit(bool);

protected:
    /**
     * @brief 初始化输入参数
     * @return 初始化结果
     * @retval 0 初始化成功
     */
    virtual int initInputParams() = 0;

    /**
     * @brief 初始化输出参数
     * @return 初始化结果
     * @retval 0 初始化成功
     */
    virtual int initOutputParams() = 0;

    /**
     * @brief 初始化工具设置
     * @return 初始化结果
     * @retval 0 初始化成功
     */
    virtual int initSettings();

    /**
     * @brief 子类在初始化中额外的初始化
     * @return 初始化结果
     * @retval 0 初始化成功
     */
    virtual int doInInit();

    /**
     * @brief 工具具体的运行处理, 由每个工具子类实现
     * @return 运行处理结果 [status, msg]
     */
    virtual std::tuple<int, QString> doInProcess() = 0;

protected slots:
    /**
     * @brief 工具输入参数改变后发出信号, 根据设置项 `RunAfterChanged` 决定是否运行工具
     */
    void onRunAfterChanged();

    /**
     * @brief 工具设置项改变后发出信号, 本处只对 `RunAfterChanged` 进行赋值处理
     * @param [in] key 设置项名称
     * @param [in] value 设置项值
     */
    void onSettingChanged(const QString &key, const QVariant &value);

private:
    Q_DISABLE_COPY(AbstractQuickTool)

    /**
     * @brief 检查输入、输出参数
     * @return 检查结果
     * @retval 0 成功
     */
    int checkParams();

    /**
     * @brief 检查输入参数是否初始化, 没有则进行初始化
     * @return 检查结果
     * @retval 0 成功
     */
    int checkInputParams();

    /**
     * @brief 检查输出参数是否初始化, 没有则进行初始化
     * @return 检查结果
     * @retval 0 成功
     */
    int checkOutputParams();

    /**
     * @brief 检查设置项是否初始化, 没有则进行初始化
     * @return 检查结果
     * @retval 0 成功
     */
    int checkSettings();

    /**
     * @brief 工具预处理, 初始化, 清除之前的结果, 发射 @ref started 信号
     * @return
     */
    bool preprocess();

    /**
     * @brief 工具处理, 调用 @ref doInProcess
     * @return 工具处理结果 [status, msg]
     */
    std::tuple<int, QString> process();

    /**
     * @brief 工具后处理, 设置结果, 发射 @ref finished 信号
     * @param [in] res 工具处理结果
     */
    void postprocess(const std::tuple<int, QString> &res);

    /// 工具整体运行时间 (ms)
    double wall_clock_time_{0.};

    /// 工具每部分算法运行时间 (ms)
    QList<double> algorithm_time_array_;

    /// 是否在参数改变后运行工具
    bool run_after_changed{true};

    QQmlEngine *qmlEngine_{nullptr};
    QJSEngine  *jsEngine_{nullptr};

    QuickToolHelper *helper_{nullptr};

    bool is_init_{false};

    /// 工具运行进度
    double progress_{0.};

    /// 工具实例唯一标识
    QString uuid_;

    std::atomic<bool> running_{false};

signals:
    /**
     * @brief 工具运行开始信号, 在向线程池提交任务时发出
     */
    void start();

    /**
     * @brief 工具运行已经开始信号, 预处理完成后发出
     */
    void started();

    /**
     * @brief 工具运行结束信号, 后处理完成后发出
     */
    void finished();

    /**
     * @brief 进度条改变信号
     */
    void progressChanged();

    /**
     * @brief 显示消息信号
     * @param [in] 消息级别
     * @param [in] 消息
     */
    void showMessage(int, const QString &);
};

/**
 * @brief 模板类, 继承 AbstractQuickTool, 根据传入输入、输出参数、设置类型实例化
 *        链接 @ref InputParams::runAfterChanged 和 @ref AbstractQuickToolSettings::settingChanged 信号
 */
template<class _InputParams, class _OutputParams, class _Settings>
class QUICKTOOLS_CORE_EXPORT AbstractTool : public AbstractQuickTool
{
    static_assert(std::is_base_of<InputParams, _InputParams>::value,
                  "_InputParams must be subclass of AbstractInputParams");
    static_assert(std::is_base_of<OutputParams, _OutputParams>::value,
                  "OutputParams must be subclass of AbstractOutputParams");
    static_assert(std::is_base_of<AbstractQuickToolSettings, _Settings>::value,
                  "Settings must be subclass of AbstractQuickToolSettings");

public:
    AbstractTool(QObject *parent = nullptr)
        : AbstractQuickTool(parent)
    {
        input_params_  = new _InputParams(this);
        output_params_ = new _OutputParams(this);
        settings_      = new _Settings(this);
        connect(input_params_, &InputParams::runAfterChanged, this,
                &AbstractTool<_InputParams, _OutputParams, _Settings>::onRunAfterChanged);
        connect(settings_, &AbstractQuickToolSettings::settingChanged, this,
                &AbstractTool<_InputParams, _OutputParams, _Settings>::onSettingChanged);
    }

    virtual ~AbstractTool() {}

    InputParams *inputParams() override
    {
        return getInputParams();
    }

    _InputParams *getInputParams()
    {
        return input_params_;
    }

    OutputParams *outputParams() override
    {
        return getOutputParams();
    }

    _OutputParams *getOutputParams()
    {
        return output_params_;
    }

    AbstractQuickToolSettings *settings() override
    {
        return settings_;
    }

    _Settings getSettings()
    {
        return settings_;
    }

protected:
    _InputParams  *input_params_{nullptr};
    _OutputParams *output_params_{nullptr};
    _Settings     *settings_{nullptr};
};

} // namespace quicktools::core
