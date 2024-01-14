#include "abstractquicktool.h"

namespace quicktools::core {

QuickToolFactor *QuickToolFactor::instance_ = nullptr;

AbstractQuickTool::AbstractQuickTool(QObject *parent)
    : QObject(parent)
{
}

AbstractQuickTool::~AbstractQuickTool()
{
    qDebug() << __FUNCTION__ << this;
}

bool AbstractQuickTool::setInputParams(AbstractInputParams *input_params)
{
    if (input_params_ != input_params)
    {
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
        output_params_ = output_params;
        emit outputParamsChanged();
        return true;
    }
    return false;
}

AbstractQuickTool *QuickToolFactor::createQuickTool(int type) const
{
    auto found = tool_creators_.find(type);
    if (found != tool_creators_.end())
    {
        auto callable = found->second;
        return callable();
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
