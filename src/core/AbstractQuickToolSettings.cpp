#include "AbstractQuickToolSettings.h"

#include "priv/Predefined.h"

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
        {  SettingsRole::GroupNameRole,   "groupName"},
        {       SettingsRole::DescRole,        "desc"},
        {       SettingsRole::NameRole,        "name"},
        {SettingsRole::DisplayNameRole, "displayName"},
        {       SettingsRole::TypeRole,        "type"},
        {    SettingsRole::VisibleRole,     "visible"},
        {      SettingsRole::ValueRole,       "value"},
        {    SettingsRole::DisplayRole,     "display"},
        { SettingsRole::AdditionalRole,  "additional"},
    };
}

QVariant AbstractQuickToolSettings::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();
    const QString &setting_name = settings_names_[index.row()];
    auto           setting_data = settings_data_.find(setting_name);
    if (role == SettingsRole::GroupNameRole)
    {
        const int group = setting_data->value(SettingsRole::GroupRole, -1).toInt();
        return groupName(group);
    }
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
        emit dataChanged(index, index, {role});
        emit settingChanged(setting_name, value);
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
                                           const QString &desc, const int type, const QVariant &value,
                                           const QVariant &additional, const bool &visible)
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
        { SettingsRole::AdditionalRole,            additional},
        {       SettingsRole::DescRole,                  desc},
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

QString AbstractQuickToolSettings::groupName(const int group) const
{
    auto found = groups_.find(group);
    if (found == groups_.end())
        return "";
    return found.value();
}

std::tuple<int, QString> AbstractQuickToolSettings::copyFrom(AbstractQuickToolSettings *other, const int group)
{
    if (other == nullptr)
        return {-1, tr("复制目标为空")};
    auto settings = other->settings(group);
    addGroup(group, other->groupName(group));
    for (auto iter = settings.constBegin(); iter != settings.constEnd(); ++iter)
    {
        const auto setting = iter.value();
        addSetting(setting.value(SettingsRole::GroupRole).toInt(), setting.value(SettingsRole::NameRole).toString(),
                   setting.value(SettingsRole::DisplayNameRole).toString(),
                   setting.value(SettingsRole::DescRole).toString(), setting.value(SettingsRole::TypeRole).toInt(),
                   setting.value(SettingsRole::ValueRole), setting.value(SettingsRole::AdditionalRole),
                   setting.value(SettingsRole::VisibleRole).toBool());
    }

    return {0, tr("复制设置成功")};
}

bool AbstractQuickToolSettings::addToogleSwitchSetting(const int group, const QString &name,
                                                       const QString &display_name, const QString &desc,
                                                       const QVariant &value)
{
    return addSetting(group, name, display_name, desc, SettingsType::ToggleSwitchType, value, QVariant(), true);
}

bool AbstractQuickToolSettings::addSliderSetting(const int group, const QString &name, const QString &display_name,
                                                 const QString &desc, const QVariant &value, const QVariant &from,
                                                 const QVariant &to, const QVariant &step_size)
{
    QVariantMap data{
        {    "from",      from},
        {      "to",        to},
        {"stepSize", step_size},
    };
    return addSetting(group, name, display_name, desc, SettingsType::SliderType, value, data, true);
}

bool AbstractQuickToolSettings::addColorDialogSetting(const int group, const QString &name, const QString &display_name,
                                                      const QString &desc, const QVariant &value)
{
    return addSetting(group, name, display_name, desc, SettingsType::ColorDialogType, value, QVariant(), true);
}

bool AbstractQuickToolSettings::addIntInputSetting(const int group, const QString &name, const QString &display_name,
                                                   const QString &desc, const QVariant &value, const QVariant &from,
                                                   const QVariant &to)
{
    QVariantMap data{
        {    "from",      from},
        {      "to",        to},
    };
    return addSetting(group, name, display_name, desc, SettingsType::IntInputType, value, data, true);
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
    addGroup(SettingsGroup::BasicGroup, tr("基础设置"));
    addToogleSwitchSetting(SettingsGroup::BasicGroup, RUN_AFTER_CHANGED, tr("改变后运行"), tr("输入参数改变后运行工具"),
                           true);
}

} // namespace quicktools::core
