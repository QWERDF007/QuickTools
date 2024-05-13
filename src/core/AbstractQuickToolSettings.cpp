#include "AbstractQuickToolSettings.h"

namespace quicktools::core {

GlobalSettings *GlobalSettings::instance_ = nullptr;

AbstractQuickToolSettings::AbstractQuickToolSettings(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(&property_data_, &QQmlPropertyMap::valueChanged, this, &AbstractQuickToolSettings::onPropertyValueChanged);
}

AbstractQuickToolSettings::~AbstractQuickToolSettings() {}

int AbstractQuickToolSettings::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return std::min(settings_names_.size(), settings_data_.size());
}

QHash<int, QByteArray> AbstractQuickToolSettings::roleNames() const
{
    return {
        {      SettingsRole::IndexRole,       "index"},
        {      SettingsRole::GroupRole,       "group"},
        {       SettingsRole::NameRole,        "name"},
        {SettingsRole::DisplayNameRole, "displayName"},
        {       SettingsRole::TypeRole,        "type"},
        {    SettingsRole::VisibleRole,     "visible"},
        {      SettingsRole::ValueRole,       "value"},
        {    SettingsRole::DisplayRole,     "display"},
        {      SettingsRole::RangeRole,       "range"},
    };
}

QVariant AbstractQuickToolSettings::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();
    const QString &setting_name = settings_names_[index.row()];
    auto           setting_data = settings_data_.find(setting_name);
    return setting_data->value(role, QVariant());
}

bool AbstractQuickToolSettings::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (index.row() < 0 || index.row() >= rowCount())
        return false;
    const QString &setting_name = settings_names_[index.row()];
    if (settings_data_[setting_name].find(role) == settings_data_[setting_name].end())
        return false;
    if (role == SettingsRole::ValueRole)
    {
        settings_data_[setting_name][role] = value;
        property_data_.insert(setting_name, value);
    }
    return false;
}

bool AbstractQuickToolSettings::addGroup(const int group, const QString &group_name)
{
    if (groups_.contains(group))
        return false;
    groups_.insert(group, group_name);
    return true;
}

bool AbstractQuickToolSettings::addSetting(const int group, const QString &name, const QString &display_name,
                                           const int type, const QVariant &value, const QVariant &range,
                                           const bool &visible)
{
    if (settings_names_.contains(name))
        return false;
    settings_names_.append(name);
    property_data_.insert(name, value);
    QMap<int, QVariant> setting{
        {      SettingsRole::IndexRole, settings_data_.size()},
        {      SettingsRole::GroupRole,                 group},
        {       SettingsRole::NameRole,                  name},
        {SettingsRole::DisplayNameRole,          display_name},
        {       SettingsRole::TypeRole,                  type},
        {    SettingsRole::VisibleRole,               visible},
        {      SettingsRole::ValueRole,                 value},
        {      SettingsRole::RangeRole,                 range},
    };
    settings_data_.insert(name, setting);
    return true;
}

QMap<QString, QMap<int, QVariant>> AbstractQuickToolSettings::settings(const int group) const
{
    if (group == -1)
        return settings_data_;
    QMap<QString, QMap<int, QVariant>> settings_data;
    for (auto iter = settings_data_.constBegin(); iter != settings_data_.constEnd(); ++iter)
    {
        const auto setting = iter.value();
        if (setting.value(SettingsRole::GroupRole).toInt() == group)
            settings_data.insert(iter.key(), iter.value());
    }
    return settings_data;
}

std::tuple<int, QString> AbstractQuickToolSettings::copyFrom(AbstractQuickToolSettings *other, const int group)
{
    if (other == nullptr)
        return {-1, "复制目标为空"};
    auto settings = other->settings(group);
    for (auto iter = settings.constBegin(); iter != settings.constEnd(); ++iter)
    {
        const auto setting = iter.value();
        addSetting(setting.value(SettingsRole::GroupRole).toInt(), setting.value(SettingsRole::NameRole).toString(),
                   setting.value(SettingsRole::DisplayNameRole).toString(),
                   setting.value(SettingsRole::TypeRole).toInt(), setting.value(SettingsRole::ValueRole),
                   setting.value(SettingsRole::RangeRole), setting.value(SettingsRole::VisibleRole).toBool());
    }

    return {0, "复制设置成功"};
}

void AbstractQuickToolSettings::onPropertyValueChanged(const QString &key, const QVariant &value)
{
    int row = settings_names_.indexOf(key);
    if (row != -1)
        setData(createIndex(row, 0), value, SettingsRole::ValueRole);
}

GlobalSettings::GlobalSettings(QObject *parent)
    : AbstractQuickToolSettings(parent)
{
    addBasicSettings();
}

void GlobalSettings::addBasicSettings()
{
    addGroup(SettingsGroup::Basic, "Basic Settings");
    addSetting(SettingsGroup::Basic, "Run After Changed", "改变后运行", SettingsType::CheckBoxType, true, QVariant(),
               true);
}

} // namespace quicktools::core
