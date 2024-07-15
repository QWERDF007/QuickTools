#pragma once

#include "CoreGlobal.h"
#include "QuickToolHelper.h"
#include "QuickToolParams.h"
#include "QuickToolSettings.h"

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

    Q_PROPERTY(InputParams *inputParams READ inputParams CONSTANT FINAL)
    Q_PROPERTY(OutputParams *outputParams READ outputParams CONSTANT FINAL)
    Q_PROPERTY(AbstractQuickToolSettings *settings READ settings CONSTANT FINAL)
    Q_PROPERTY(QuickToolHelper *helper READ helper CONSTANT FINAL)
    Q_PROPERTY(QString name READ name CONSTANT FINAL) // FINAL 表明该属性不会被派生类覆盖
    Q_PROPERTY(QString doc READ doc CONSTANT FINAL)
    Q_PROPERTY(double progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QString uuid READ uuid CONSTANT FINAL)
public:
    AbstractQuickTool(QObject *parent = nullptr);
    virtual ~AbstractQuickTool();

    virtual QString name() const
    {
        return "AbstractQuickTool";
    }

    virtual QString doc() const = 0;

    int init();

    QString uuid() const
    {
        return uuid_;
    }

    void run() override;

    virtual InputParams  *inputParams()  = 0;
    virtual OutputParams *outputParams() = 0;

    virtual AbstractQuickToolSettings *settings() = 0;

    double progress() const
    {
        return progress_;
    }

    bool setProgress(const double v);

    /**
     * @brief 添加一个算法运行时间 (ms)
     * @param algorithm_time
     */
    void addAlgorithmTime(const double &algorithm_time)
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

    bool        isInit() const;
    void        setIsInit(bool);
    virtual int doInInit();

    virtual std::tuple<int, QString> doInProcess() = 0;

protected slots:
    void onRunAfterChanged();
    void onSettingChanged(const QString &key, const QVariant &value);

private:
    Q_DISABLE_COPY(AbstractQuickTool)

    int checkParams();
    int checkInputParams();
    int checkOutputParams();
    int checkSettings();

    bool preprocess();
    std::tuple<int, QString> process();
    void postprocess(const std::tuple<int, QString>& res);

    double        wall_clock_time_{0.};
    QList<double> algorithm_time_array_;

    bool run_after_changed{true};

    QQmlEngine *qmlEngine_{nullptr};
    QJSEngine  *jsEngine_{nullptr};

    QuickToolHelper *helper_{nullptr};

    bool is_init_{false};

    double  progress_{0.};
    QString uuid_;

signals:
    void start();
    void started();
    void finished();
    void progressChanged();
    void showMessage(int, const QString &);
};

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
