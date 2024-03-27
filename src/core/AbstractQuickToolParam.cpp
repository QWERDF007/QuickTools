#include "AbstractQuickToolParam.h"

namespace quicktools::core {

using paramtypes::QuickToolParamRole;
using paramtypes::QuickToolParamType;

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
        {    QuickToolParamRole::ParamNameRole,     "paramName"},
        {    QuickToolParamRole::ParamTypeRole,     "paramType"},
        {QuickToolParamRole::ParamTypeNameRole, "paramTypeName"},
        {QuickToolParamRole::ParamEditableRole, "paramEditable"},
        { QuickToolParamRole::ParamVisibleRole,  "paramVisible"},
        {   QuickToolParamRole::ParamValueRole,    "paramValue"},
        {   QuickToolParamRole::ParamRangeRole,    "paramRange"},
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

bool AbstractToolParams::addParam(const QString &en_name, const QString &zh_name, const int type,
                                  const bool is_property, const bool run_after_changed, const bool editable,
                                  const QVariant &visible, const QVariant &value, const QVariant &range)
{
    if (params_names_.contains(en_name))
    {
        return false;
    }
    params_names_.append(en_name);
    QMap<int, QVariant> param{
        {           QuickToolParamRole::ParamNameRole,           zh_name},
        {           QuickToolParamRole::ParamTypeRole,              type},
        {       QuickToolParamRole::ParamTypeNameRole, getTypeName(type)},
        {        QuickToolParamRole::ParamVisibleRole,           visible},
        {          QuickToolParamRole::ParamValueRole,             value},
        {          QuickToolParamRole::ParamRangeRole,             range},
        {     QuickToolParamRole::ParamIsPropertyRole,       is_property},
        {       QuickToolParamRole::ParamEditableRole,          editable},
        {QuickToolParamRole::RunAfterParamChangedRole, run_after_changed},
    };
    params_data_.insert(en_name, param);
    if (is_property)
    {
        property_data_.insert(en_name, value);
    }
    return true;
}

QString AbstractToolParams::getTypeName(const int type)
{
    static QMap<int, QString> typeNamesMap{
        {QuickToolParamType::ParamTextType, "Text"},
    };
    auto found = typeNamesMap.find(type);
    return found == typeNamesMap.end() ? "undefined" : found.value();
}

void AbstractToolParams::onPropertyValueChanged(const QString &key, const QVariant &value)
{
    qInfo() << __FUNCTION__ << key << value;
    setData(key, value);
}

AbstractOutputParams::AbstractOutputParams(QObject *parent)
    : AbstractToolParams(parent)
{
    addParam("Status", "状态", QuickToolParamType::ParamIntType, false, true, QVariant());
    addParam("Time", "时间", QuickToolParamType::ParamDouble1DArrayType, false, true, QVariant());
}

bool AbstractOutputParams::addParam(const QString &en_name, const QString &zh_name, const int type,
                                    const bool is_property, const QVariant &value, const QVariant &visible,
                                    const QVariant &range)
{
    return AbstractToolParams::addParam(en_name, zh_name, type, is_property, false, false, visible, value, range);
}

bool AbstractOutputParams::setToolTime(const double wall_clock_time, const double algorithm_time)
{
    return setData("Time", QVariantList{wall_clock_time, algorithm_time});
}

bool AbstractOutputParams::setStatus(const int status, const QString &msg)
{
    return setData("Status", QVariantList{status, msg});
}

} // namespace quicktools::core
