#include "AbstractQuickToolSettings.h"

namespace quicktools::core {

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
    return settings_.size();
}

QHash<int, QByteArray> AbstractQuickToolSettings::roleNames() const
{
    return {

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

} // namespace quicktools::core
