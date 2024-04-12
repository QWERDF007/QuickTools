#include "AbstractQuickTool.h"

#include <chrono>

namespace quicktools::core {

QuickToolFactor *QuickToolFactor::instance_ = nullptr;
QQmlEngine *QuickToolFactor::qmlEngine_ = nullptr;
QJSEngine *QuickToolFactor::jsEngine_ = nullptr;

AbstractQuickTool::AbstractQuickTool(QObject *parent)
    : QObject{parent}
{
    setAutoDelete(false);
    // connect(input_params_, &AbstractInputParams::quicktoolRun, this, &AbstractQuickTool::run);
}

AbstractQuickTool::~AbstractQuickTool()
{
    qDebug() << __FUNCTION__ << this;
}

int AbstractQuickTool::init()
{
    return checkParams();
}

void AbstractQuickTool::run()
{
    int ret = checkParams();
    if (ret != 0)
    {
        return;
    }
    emit started();
    auto start_time           = std::chrono::high_resolution_clock::now();
    const auto &[status, msg] = exec();
    auto end_time             = std::chrono::high_resolution_clock::now();
    wall_clock_time_          = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    output_params_->setToolTime(wall_clock_time_, algorithm_time_);
    output_params_->setStatus(status, msg);
    emit finished();
}

bool AbstractQuickTool::setInputParams(AbstractInputParams *input_params)
{
    if (input_params_ != input_params)
    {
        if (input_params_ != nullptr)
        {
            delete input_params_;
        }
        input_params_ = input_params;
        emit inputParamsChanged();
        return true;
    }
    return false;
}

bool AbstractQuickTool::setOutputParams(AbstractOutputParams *output_params)
{
    if (output_params_ != output_params)
    {
        if (output_params_ != nullptr)
        {
            delete output_params_;
        }
        output_params_ = output_params;
        emit outputParamsChanged();
        return true;
    }
    return false;
}

void AbstractQuickTool::setEngine(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    qmlEngine_ = qmlEngine;
    jsEngine_ = jsEngine;
}

void AbstractQuickTool::submit()
{
    QThreadPool::globalInstance()->start(this);
}

int AbstractQuickTool::checkParams()
{
    int ret = checkInputParams();
    if (ret != 0)
    {
        return ret;
    }
    ret = checkOutputParams();
    return ret;
}

int AbstractQuickTool::checkInputParams()
{
    if (input_params_ == nullptr)
    {
        return -1;
    }
    if (input_params_->empty() || !input_params_->isInit())
    {
        int ret = initInputParams();
        if (ret == 0)
            input_params_->setIsInit(true);
        return ret;
    }
    return 0;
}

int AbstractQuickTool::checkOutputParams()
{
    if (output_params_ == nullptr)
    {
        return -1;
    }
    if (output_params_->empty() || !output_params_->isInit())
    {
        int ret = initOutputParams();
        if (ret == 0)
            output_params_->setIsInit(true);
        return ret;
    }
    return 0;
}

AbstractQuickTool *QuickToolFactor::createQuickTool(int type, QObject *parent) const
{
    auto found = tool_creators_.find(type);
    if (found != tool_creators_.end())
    {
        auto               callable   = found->second;
        AbstractQuickTool *quick_tool = callable(parent);
        if (quick_tool)
        {
            quick_tool->init();
            quick_tool->setEngine(qmlEngine_, jsEngine_);
        }
        return quick_tool;
    }
    return nullptr;
}

void QuickToolFactor::registerQuickTool(int type, AbstractQuickToolCreator creator)
{
    auto found = tool_creators_.find(type);
    assert(found == tool_creators_.end() && "This type is already registerd");
    tool_creators_.emplace(type, creator);
}

} // namespace quicktools::core
