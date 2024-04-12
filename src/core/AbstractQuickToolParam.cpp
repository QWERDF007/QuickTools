#include "AbstractQuickToolParam.h"

#include <QStringBuilder>


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
        {      QuickToolParamRole::ParamIndexRole,       "paramIndex"},
        {       QuickToolParamRole::ParamNameRole,        "paramName"},
        {QuickToolParamRole::ParamDisplayNameRole, "paramDisplayName"},
        {       QuickToolParamRole::ParamTypeRole,        "paramType"},
        {   QuickToolParamRole::ParamTypeNameRole,    "paramTypeName"},
        {   QuickToolParamRole::ParamEditableRole,    "paramEditable"},
        {    QuickToolParamRole::ParamVisibleRole,     "paramVisible"},
        {      QuickToolParamRole::ParamValueRole,       "paramValue"},
        {    QuickToolParamRole::ParamDisplayRole,     "paramDisplay"},
        {      QuickToolParamRole::ParamRangeRole,       "paramRange"},
    };
}

namespace {

QVariant statusParamDisplay(const QVariant &value)
{
    const QVariantList &data = value.toList();
    if (data.size() != 2)
        return "";
    return QString("[%1, %2]").arg(data[0].toInt()).arg(data[1].toString());
}

QVariant intParamDisplay(const QVariant &value)
{
    return value;
}

QVariant doubleParamDisplay(const QVariant &value)
{
    return value;
}

QVariant textParamDisplay(const QVariant &value)
{
    return value;
}

QVariant int1DArrayParamDisplay(const QVariant &value)
{
    const QVariantList &data = value.toList();
    if (data.empty())
        return "";
    const int size             = data.size();
    const int array_last_index = size - 1;
    QString   array_text       = "[";
    for (int i = 0; i < size; ++i)
    {
        array_text += i < array_last_index ? QString("%1, ").arg(data[i].toInt()) : QString::number(data[i].toInt());
    }
    array_text += "]";
    return array_text;
}

QVariant int2DArrayParamDisplay(const QVariant &value)
{
    const QVariantList &dataArray = value.toList();
    if (dataArray.empty())
        return "";
    const int rows             = dataArray.size();
    const int array_last_index = rows - 1;
    QString   array_text       = "[";
    for (int i = 0; i < rows; ++i)
    {
        const QVariantList &data = dataArray[i].toList();
        if (data.empty())
            continue;
        const int cols                 = data.size();
        const int sub_array_last_index = cols - 1;
        QString   sub_array_text       = "[";
        for (int j = 0; j < cols; ++j)
        {
            sub_array_text
                += j < sub_array_last_index ? QString("%1, ").arg(data[j].toInt()) : QString::number(data[j].toInt());
        }
        sub_array_text += "]";
        array_text += i < array_last_index ? sub_array_text + ", " : sub_array_text;
    }
    array_text += "]";
    return array_text;
}

QVariant double1DArrayParamDisplay(const QVariant &value)
{
    const QVariantList &data = value.toList();
    if (data.empty())
        return "";
    const int size             = data.size();
    const int array_last_index = size - 1;
    QString   array_text       = "[";
    for (int i = 0; i < size; ++i)
    {
        array_text += i < array_last_index ? QString("%1, ").arg(data[i].toDouble(), 0, 'g', 4)
                                           : QString("%1").arg(data[i].toDouble(), 0, 'g', 4);
    }
    array_text += "]";
    return array_text;
}

QVariant double2DArrayParamDisplay(const QVariant &value)
{
    const QVariantList &dataArray = value.toList();
    if (dataArray.empty())
        return "";
    const int rows             = dataArray.size();
    const int array_last_index = rows - 1;
    QString   array_text       = "[";
    for (int i = 0; i < rows; ++i)
    {
        const QVariantList &data = dataArray[i].toList();
        if (data.empty())
            continue;
        const int cols                 = data.size();
        const int sub_array_last_index = cols - 1;
        QString   sub_array_text       = "[";
        for (int j = 0; j < cols; ++j)
        {
            sub_array_text += j < sub_array_last_index ? QString("%1, ").arg(data[j].toDouble(), 0, 'g', 4)
                                                       : QString("%1").arg(data[j].toDouble(), 0, 'g', 4);
        }
        sub_array_text += "]";
        array_text += i < array_last_index ? sub_array_text + ", " : sub_array_text;
    }
    array_text += "]";
    return array_text;
}

QVariant text1DArrayParamDisplay(const QVariant &value)
{
    const QVariantList &data = value.toList();
    if (data.empty())
        return "";
    const int size             = data.size();
    const int array_last_index = size - 1;
    QString   array_text       = "[";
    for (int i = 0; i < size; ++i)
    {
        array_text += i < array_last_index ? QString("%1, ").arg(data[i].toString()) : data[i].toString();
    }
    array_text += "]";
    return array_text;
}

QVariant text2DArrayParamDisplay(const QVariant &value)
{
    const QVariantList &dataArray = value.toList();
    if (dataArray.empty())
        return "";
    const int rows             = dataArray.size();
    const int array_last_index = rows - 1;
    QString   array_text       = "[";
    for (int i = 0; i < rows; ++i)
    {
        const QVariantList &data = dataArray[i].toList();
        if (data.empty())
            continue;
        const int cols                 = data.size();
        const int sub_array_last_index = cols - 1;
        QString   sub_array_text       = "[";
        for (int j = 0; j < cols; ++j)
        {
            sub_array_text += j < sub_array_last_index ? QString("%1, ").arg(data[j].toString()) : data[j].toString();
        }
        sub_array_text += "]";
        array_text += i < array_last_index ? sub_array_text + ", " : sub_array_text;
    }
    array_text += "]";
    return array_text;
}

QVariant imageParamDisplay(const QVariant &value)
{
    return value;
}

QVariant getParamsDisplay(const int param_type, const QVariant &data)
{
    static std::map<int, std::function<QVariant(const QVariant &)>> func_map{
        {       QuickToolParamType::ParamStatusType,        statusParamDisplay},
        {          QuickToolParamType::ParamIntType,           intParamDisplay},
        {       QuickToolParamType::ParamDoubleType,        doubleParamDisplay},
        {         QuickToolParamType::ParamTextType,          textParamDisplay},
        {   QuickToolParamType::ParamInt1DArrayType,    int1DArrayParamDisplay},
        {QuickToolParamType::ParamDouble1DArrayType, double1DArrayParamDisplay},
        {  QuickToolParamType::ParamText1DArrayType,   text1DArrayParamDisplay},
        {   QuickToolParamType::ParamInt2DArrayType,    int2DArrayParamDisplay},
        {QuickToolParamType::ParamDouble2DArrayType, double2DArrayParamDisplay},
        {  QuickToolParamType::ParamText2DArrayType,   text2DArrayParamDisplay},
        {        QuickToolParamType::ParamImageType,         imageParamDisplay},
    };
    if (data.isNull())
        return "";
    auto found = func_map.find(param_type);
    if (found == func_map.end())
        return QVariant();
    auto callable = found->second;
    return callable(data);
}

} // namespace

QVariant AbstractToolParams::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();
    const QString &param_name = params_names_[index.row()];
    auto           param_data = params_data_.find(param_name);
    if (param_data == params_data_.end())
        return QVariant();
    if (role == QuickToolParamRole::ParamDisplayRole)
    {
        return getParamsDisplay(param_data->value(QuickToolParamRole::ParamTypeRole).toInt(),
                                param_data->value(QuickToolParamRole::ParamValueRole));
    }
    return param_data->value(role, QVariant());
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
    {
        const QString &param_name      = params_names_[index.row()];
        params_data_[param_name][role] = value;
        const bool is_property         = params_data_[param_name][QuickToolParamRole::ParamIsPropertyRole].toBool();
        if (is_property)
        {
            property_data_.insert(param_name, value);
        }
        emit dataChanged(index, index, {QuickToolParamRole::ParamVisibleRole});
        return true;
        break;
    }
    case QuickToolParamRole::ParamValueRole:
    {
        const QString &param_name      = params_names_[index.row()];
        params_data_[param_name][role] = value;
        const bool is_property         = params_data_[param_name][QuickToolParamRole::ParamIsPropertyRole].toBool();
        if (is_property)
        {
            property_data_.insert(param_name, value);
        }
        emit dataChanged(index, index, {QuickToolParamRole::ParamValueRole, QuickToolParamRole::ParamDisplayRole});

        const bool run_after_changed = params_data_[param_name][QuickToolParamRole::RunAfterParamChangedRole].toBool();
        if (run_after_changed)
        {
            emit runAfterChanged();
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

bool AbstractToolParams::addParam(const QString &en_name, const QString &zh_name, const int type, const QVariant &value,
                                  const QVariant &range, const bool editable, const bool is_property,
                                  const bool run_after_changed, const bool &visible)
{
    if (params_names_.contains(en_name))
    {
        return false;
    }
    params_names_.append(en_name);
    QMap<int, QVariant> param{
        {          QuickToolParamRole::ParamIndexRole, params_data_.size()},
        {           QuickToolParamRole::ParamNameRole,             en_name},
        {    QuickToolParamRole::ParamDisplayNameRole,             zh_name},
        {           QuickToolParamRole::ParamTypeRole,                type},
        {       QuickToolParamRole::ParamTypeNameRole,   getTypeName(type)},
        {        QuickToolParamRole::ParamVisibleRole,             visible},
        {          QuickToolParamRole::ParamValueRole,               value},
        {          QuickToolParamRole::ParamRangeRole,               range},
        {     QuickToolParamRole::ParamIsPropertyRole,         is_property},
        {       QuickToolParamRole::ParamEditableRole,            editable},
        {QuickToolParamRole::RunAfterParamChangedRole,   run_after_changed},
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
        {       QuickToolParamType::ParamStatusType,          "Status"},
        {          QuickToolParamType::ParamIntType,             "Int"},
        {       QuickToolParamType::ParamDoubleType,          "Double"},
        {         QuickToolParamType::ParamTextType,            "Text"},
        {   QuickToolParamType::ParamInt1DArrayType,    "1D Int Array"},
        {QuickToolParamType::ParamDouble1DArrayType, "1D Double Array"},
        {  QuickToolParamType::ParamText1DArrayType,   "1D Text Array"},
        {   QuickToolParamType::ParamInt2DArrayType,    "2D Int Array"},
        {QuickToolParamType::ParamDouble2DArrayType, "2D Double Array"},
        {  QuickToolParamType::ParamText2DArrayType,   "2D Text Array"},
        {        QuickToolParamType::ParamImageType,           "Image"},
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
    addParam("Status", "运行状态", QuickToolParamType::ParamStatusType, QVariant(), QVariant(), false, true);
    addParam("Time", "运行时间", QuickToolParamType::ParamDouble1DArrayType, QVariant(), QVariant(), false, true);
}

bool AbstractOutputParams::addParam(const QString &en_name, const QString &zh_name, const int type,
                                    const QVariant &value, const QVariant &range, const bool is_property,
                                    const bool &visible)
{
    return AbstractToolParams::addParam(en_name, zh_name, type, value, range, false, is_property, false, visible);
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
