#include "abstractquicktool.h"

namespace quicktools::core {

QuickToolFactor *QuickToolFactor::instance_ = nullptr;

AbstractQuickTool::AbstractQuickTool(QObject *parent)
    : QObject(parent)
{
    // connect(input_params_, &AbstractInputParams::quicktoolRun, this, &AbstractQuickTool::run);
}

AbstractQuickTool::~AbstractQuickTool()
{
    qDebug() << __FUNCTION__ << this;
}

int AbstractQuickTool::exec()
{
    int ret = checkParams();
    if (ret != 0)
    {
        return ret;
    }
    return run();
}

int AbstractQuickTool::init()
{
    return checkParams();
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
    if (input_params_->empty())
    {
        return initInputParams();
    }
    return 0;
}

int AbstractQuickTool::checkOutputParams()
{
    if (output_params_ == nullptr)
    {
        return -1;
    }
    if (output_params_->empty())
    {
        return initOutputParams();
    }
    return 0;
}

AbstractQuickTool *QuickToolFactor::createQuickTool(int type) const
{
    auto found = tool_creators_.find(type);
    if (found != tool_creators_.end())
    {
        auto callable = found->second;
        AbstractQuickTool * quick_tool = callable();
        if (quick_tool)
        {
            quick_tool->init();
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
