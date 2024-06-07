#include "core/QuickToolParams.h"

#include <QStringBuilder>

namespace quicktools::core {

using paramtypes::QuickToolParamRole;
using paramtypes::QuickToolParamType;

AbstractQuickToolParams::AbstractQuickToolParams(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(&property_data_, &QQmlPropertyMap::valueChanged, this, &AbstractQuickToolParams::onPropertyValueChanged);
}

AbstractQuickToolParams::~AbstractQuickToolParams() {}

int AbstractQuickToolParams::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return std::min(params_names_.size(), params_data_.size());
}

QHash<int, QByteArray> AbstractQuickToolParams::roleNames() const
{
    return {
        {      QuickToolParamRole::ParamIndexRole,       "paramIndex"},
        {       QuickToolParamRole::ParamNameRole,        "paramName"},
        {QuickToolParamRole::ParamDisplayNameRole, "paramDisplayName"},
        {       QuickToolParamRole::ParamDescRole,        "paramDesc"},
        {       QuickToolParamRole::ParamTypeRole,        "paramType"},
        {   QuickToolParamRole::ParamTypeNameRole,    "paramTypeName"},
        {   QuickToolParamRole::ParamEditableRole,    "paramEditable"},
        {    QuickToolParamRole::ParamVisibleRole,     "paramVisible"},
        {      QuickToolParamRole::ParamValueRole,       "paramValue"},
        {    QuickToolParamRole::ParamDisplayRole,     "paramDisplay"},
        { QuickToolParamRole::ParamAdditionalRole,  "paramAdditional"},
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

QVariant timeParamDisplay(const QVariant &value)
{
    const QVariantList &time_array       = value.toList();
    const int           size             = time_array.size();
    const int           array_last_index = size - 1;
    QString             display;
    for (int i = 0; i < size; ++i)
    {
        double v = time_array[i].toDouble();
        display += i < array_last_index ? QString("%1ms, ").arg(v, 0, 'g', 3) : QString("%1ms").arg(v, 0, 'g', 3);
    }
    return display;
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
        {       QuickToolParamType::StatusParamType,        statusParamDisplay},
        {         QuickToolParamType::TimeParamType,          timeParamDisplay},
        {          QuickToolParamType::IntParamType,           intParamDisplay},
        {       QuickToolParamType::DoubleParamType,        doubleParamDisplay},
        {         QuickToolParamType::TextParamType,          textParamDisplay},
        {   QuickToolParamType::Int1DArrayParamType,    int1DArrayParamDisplay},
        {QuickToolParamType::Double1DArrayParamType, double1DArrayParamDisplay},
        {  QuickToolParamType::Text1DArrayParamType,   text1DArrayParamDisplay},
        {   QuickToolParamType::Int2DArrayParamType,    int2DArrayParamDisplay},
        {QuickToolParamType::Double2DArrayParamType, double2DArrayParamDisplay},
        {  QuickToolParamType::Text2DArrayParamType,   text2DArrayParamDisplay},
        {        QuickToolParamType::ImageParamType,         imageParamDisplay},
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

QVariant AbstractQuickToolParams::data(const QModelIndex &index, int role) const
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

QVariant AbstractQuickToolParams::data(const QString &name, int role) const
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

bool AbstractQuickToolParams::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool AbstractQuickToolParams::setData(const QString &name, const QVariant &value)
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

bool AbstractQuickToolParams::addParam(const QString &name, const QString &display_name, const QString &desc,
                                       const int type, const QVariant &value, const QVariant &additional,
                                       const bool editable, const bool is_property, const bool run_after_changed,
                                       const bool &visible)
{
    if (params_names_.contains(name))
        return false;
    params_names_.append(name);
    QMap<int, QVariant> param{
        {          QuickToolParamRole::ParamIndexRole, params_data_.size()},
        {           QuickToolParamRole::ParamNameRole,                name},
        {    QuickToolParamRole::ParamDisplayNameRole,        display_name},
        {           QuickToolParamRole::ParamDescRole,                desc},
        {           QuickToolParamRole::ParamTypeRole,                type},
        {       QuickToolParamRole::ParamTypeNameRole,   getTypeName(type)},
        {        QuickToolParamRole::ParamVisibleRole,             visible},
        {          QuickToolParamRole::ParamValueRole,               value},
        {     QuickToolParamRole::ParamAdditionalRole,          additional},
        {     QuickToolParamRole::ParamIsPropertyRole,         is_property},
        {       QuickToolParamRole::ParamEditableRole,            editable},
        {QuickToolParamRole::RunAfterParamChangedRole,   run_after_changed},
    };
    params_data_.insert(name, param);
    if (is_property)
        property_data_.insert(name, value);
    return true;
}

QString AbstractQuickToolParams::getTypeName(const int type)
{
    static QMap<int, QString> typeNamesMap{
        {   QuickToolParamType::IntSpinBoxParamType,      "IntSpinBox"},
        {QuickToolParamType::DoubleSpinBoxParamType,   "DoubleSpinBox"},
        {     QuickToolParamType::ComboBoxParamType,        "ComboBox"},
        {       QuickToolParamType::StatusParamType,          "Status"},
        {         QuickToolParamType::TimeParamType,            "Time"},
        {          QuickToolParamType::IntParamType,             "Int"},
        {       QuickToolParamType::DoubleParamType,          "Double"},
        {         QuickToolParamType::TextParamType,            "Text"},
        {   QuickToolParamType::Int1DArrayParamType,    "1D Int Array"},
        {QuickToolParamType::Double1DArrayParamType, "1D Double Array"},
        {  QuickToolParamType::Text1DArrayParamType,   "1D Text Array"},
        {   QuickToolParamType::Int2DArrayParamType,    "2D Int Array"},
        {QuickToolParamType::Double2DArrayParamType, "2D Double Array"},
        {  QuickToolParamType::Text2DArrayParamType,   "2D Text Array"},
        {        QuickToolParamType::ImageParamType,           "Image"},
    };
    auto found = typeNamesMap.find(type);
    return found == typeNamesMap.end() ? "undefined" : found.value();
}

void AbstractQuickToolParams::onPropertyValueChanged(const QString &key, const QVariant &value)
{
    setData(key, value);
}

OutputParams::OutputParams(QObject *parent)
    : AbstractQuickToolParams(parent)
{
    addParam("Status", tr("运行状态"), tr("工具的运行状态"), QuickToolParamType::StatusParamType, QVariant(),
             QVariant(), false, true);
    addParam("Time", tr("运行时间"), tr("工具的运行时间"), QuickToolParamType::TimeParamType, QVariant(), QVariant(),
             false, true);
}

bool OutputParams::addParam(const QString &name, const QString &display_name, const QString &desc, const int type,
                            const QVariant &value, const QVariant &additional, const bool is_property,
                            const bool &visible)
{
    return AbstractQuickToolParams::addParam(name, display_name, desc, type, value, additional, false, is_property,
                                             false, visible);
}

bool OutputParams::setToolTime(const double wall_clock_time, const QList<double> &algorithm_time_array)
{
    if (algorithm_time_array.isEmpty())
        return setData("Time", QVariantList{wall_clock_time});
    QVariantList time_array{wall_clock_time};
    for (const double &v : algorithm_time_array)
    {
        time_array.append(v);
    }
    return setData("Time", time_array);
}

bool OutputParams::setStatus(const int status, const QString &msg)
{
    return setData("Status", QVariantList{status, msg});
}

} // namespace quicktools::core