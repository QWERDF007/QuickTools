#include "abstractquicktoolparam.h"

namespace quicktools::core {

using paramtypes::QuickToolParamRole;

AbstractToolParams::AbstractToolParams(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(&property_data_, &QQmlPropertyMap::valueChanged, this, &AbstractToolParams::onPropertyValueChanged);
}

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

QVariant AbstractToolParams::data(const QString &name, int role) const
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
        return data(createIndex(row, 0), role);
    }
    return QVariant();
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
        const bool is_property         = params_data_[param_name][QuickToolParamRole::ParamIsPropertyRole].toBool();
        if (is_property)
        {
            property_data_.insert(param_name, value);
        }
        emit       dataChanged(index, index, {role});
        const bool run_after_changed = params_data_[param_name][QuickToolParamRole::RunAfterParamChangedRole].toBool();
        if (run_after_changed)
        {
            emit quicktoolStart();
        }
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

bool AbstractToolParams::addParam(const QString &name, const int type, const bool is_property,
                                  const bool run_after_changed, const QVariant &visible, const QVariant &value,
                                  const QVariant &range)
{
    if (params_names_.contains(name))
    {
        return false;
    }
    params_names_.append(name);
    QMap<int, QVariant> param{
        {           QuickToolParamRole::ParamNameRole,              name},
        {           QuickToolParamRole::ParamTypeRole,              type},
        {        QuickToolParamRole::ParamVisibleRole,           visible},
        {          QuickToolParamRole::ParamValueRole,             value},
        {          QuickToolParamRole::ParamRangeRole,             range},
        {     QuickToolParamRole::ParamIsPropertyRole,       is_property},
        {QuickToolParamRole::RunAfterParamChangedRole, run_after_changed},
    };
    params_data_.insert(name, param);
    if (is_property)
    {
        property_data_.insert(name, value);
    }
    return true;
}

void AbstractToolParams::onPropertyValueChanged(const QString &key, const QVariant &value)
{
    qInfo() << __FUNCTION__ << key << value;
    setData(key, value);
}

} // namespace quicktools::core
