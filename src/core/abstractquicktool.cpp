#include "abstractquicktool.h"

#include <unordered_map>

namespace quicktools {

QuickToolFactor *QuickToolFactor::instance_ = nullptr;

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

QVariant AbstractToolParams::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= params_.size())
        return QVariant();
    return params_.at(index.row()).value(role, QVariant());
}

bool AbstractToolParams::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= params_.size())
        return false;
    switch (role)
    {
    case ParamVisibleRole:
    case ParamValueRole:
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
        {   ParamNameRole,    name},
        {   ParamTypeRole,    type},
        {ParamVisibleRole, visible},
        {  ParamValueRole,   value},
        {  ParamRangeRole,   range},
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

bool AbstractQuickTool::setImageSource(const QUrl &url)
{
    if (url == image_source_)
    {
        return false;
    }
    image_source_ = url;
    emit imageSourceChanged();
    return true;
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
