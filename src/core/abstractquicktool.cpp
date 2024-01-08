#include "abstractquicktool.h"

#include <unordered_map>

namespace quicktools {

QuickToolFactor *QuickToolFactor::instance_ = nullptr;

using quicktooltypes::QuickToolParamRole;

AbstractToolParams::~AbstractToolParams()
{
    qInfo() << __FUNCTION__ << this;
}

int AbstractToolParams::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return params_.size();
}

QHash<int, QByteArray> AbstractToolParams::roleNames() const
{
    return {
        {   QuickToolParamRole::ParamNameRole,    "name"},
        {   QuickToolParamRole::ParamTypeRole,    "type"},
        {QuickToolParamRole::ParamVisibleRole, "visible"},
        {  QuickToolParamRole::ParamValueRole,   "value"},
        {  QuickToolParamRole::ParamRangeRole,   "range"},
    };
}

QVariant AbstractToolParams::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() < 0 || index.row() >= params_.size())
        return QVariant();
    return params_.at(index.row()).value(role, QVariant());
}

bool AbstractToolParams::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (index.row() < 0 || index.row() >= params_.size())
        return false;
    switch (role)
    {
    case QuickToolParamRole::ParamVisibleRole:
    case QuickToolParamRole::ParamValueRole:
        params_[index.row()][role] = value;
        return true;
        break;
    default:
        break;
    }
    return false;
}

bool AbstractToolParams::addParam(const QString &name, const int type, const QVariant &value, const QVariant &range,
                                  const QVariant &visible)
{
    QMap<int, QVariant> param{
        {   QuickToolParamRole::ParamNameRole,    name},
        {   QuickToolParamRole::ParamTypeRole,    type},
        {QuickToolParamRole::ParamVisibleRole, visible},
        {  QuickToolParamRole::ParamValueRole,   value},
        {  QuickToolParamRole::ParamRangeRole,   range},
    };
    params_.emplace_back(param);
    return true;
}

AbstractQuickTool::AbstractQuickTool(QObject *parent)
    : QObject(parent)
{
}

AbstractQuickTool::~AbstractQuickTool()
{
    qDebug() << __FUNCTION__ << this;
}

bool AbstractQuickTool::setInputParams(AbstractToolInputParams *input_params)
{
    if (input_params_ != input_params)
    {
        input_params_ = input_params;
        emit inputParamsChanged();
        return true;
    }
    return false;
}

bool AbstractQuickTool::setOutputParams(AbstractToolOutputParams *output_params)
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

} // namespace quicktools
