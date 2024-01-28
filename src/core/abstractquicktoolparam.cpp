#include "abstractquicktoolparam.h"

namespace quicktools::core {

using paramtypes::QuickToolParamRole;

AbstractToolParams::~AbstractToolParams()
{
    qInfo() << __FUNCTION__ << this;
}

int AbstractToolParams::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return std::min(params_names_.size(), params_data_.size());
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
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();
    const QString             &param_name = params_names_[index.row()];
    const QMap<int, QVariant> &param_data = params_data_[param_name];
    return param_data.value(role, QVariant());
}

bool AbstractToolParams::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (index.row() < 0 || index.row() >= rowCount())
        return false;
    switch (role)
    {
    case QuickToolParamRole::ParamVisibleRole:
    case QuickToolParamRole::ParamValueRole:
    {
        const QString &param_name      = params_names_[index.row()];
        params_data_[param_name][role] = value;
        emit dataChanged(index, index, {role});
        return true;
        break;
    }
    default:
        break;
    }
    return false;
}

bool AbstractToolParams::setData(const QString &name, const QVariant &value)
{
    int row{-1};
    for (int i = 0; i < params_names_.size(); ++i)
    {
        if (name == params_names_[i])
        {
            row = i;
            break;
        }
    }
    if (row != -1)
    {
        return setData(createIndex(row, 0), value, QuickToolParamRole::ParamValueRole);
    }
    return false;
}

bool AbstractToolParams::addParam(const QString &name, const int type, const QVariant &value, const QVariant &range,
                                  const QVariant &visible)
{
    if (params_names_.contains(name))
    {
        return false;
    }
    params_names_.append(name);
    QMap<int, QVariant> param{
        {   QuickToolParamRole::ParamNameRole,    name},
        {   QuickToolParamRole::ParamTypeRole,    type},
        {QuickToolParamRole::ParamVisibleRole, visible},
        {  QuickToolParamRole::ParamValueRole,   value},
        {  QuickToolParamRole::ParamRangeRole,   range},
    };
    params_data_.insert(name, param);
    return true;
}

} // namespace quicktools::core
