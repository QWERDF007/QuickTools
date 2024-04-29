#include "AbstractQuickToolSettings.h"

namespace quicktools::core {

GlobalSettings * GlobalSettings::instance_ = nullptr;

AbstractQuickToolSettings::AbstractQuickToolSettings(QObject *parent) : QAbstractListModel(parent)
{

}

AbstractQuickToolSettings::~AbstractQuickToolSettings()
{

}

int AbstractQuickToolSettings::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return std::min(settings_names_.size(), settings_data_.size());
}

QHash<int, QByteArray> AbstractQuickToolSettings::roleNames() const
{
    return {
        {IndexRole, "index"},
        {GroupRole, "group"},
        {NameRole, "name"},
        {DisplayNameRole, "displayName"},
        {TypeRole, "type"},
        {VisibleRole, "visible"},
        {ValueRole, "value"},
        {DisplayRole, "display"},
        {RangeRole, "range"},
    };
}

QVariant AbstractQuickToolSettings::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)
    return QVariant();
}

bool AbstractQuickToolSettings::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

void AbstractQuickToolSettings::onPropertyValueChanged(const QString &key, const QVariant &value)
{
    int row = settings_names_.indexOf(key);
    if (row != -1)
    {
        setData(createIndex(row, 0), value, ValueRole);
    }
}

GlobalSettings::GlobalSettings(QObject *parent) : AbstractQuickToolSettings(parent)
{
    // addSetting("")
}

} // namespace quicktools::core
