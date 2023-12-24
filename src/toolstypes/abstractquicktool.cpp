#include "abstractquicktool.h"

namespace quicktools {

QHash<int, QByteArray> quicktools::AbstractToolParams::roleNames() const
{
    return {
        {ParamRole::ParamNameRole, "name"},
        {ParamRole::ParamTypeRole, "type"},
        {ParamRole::ParamValueRole, "value"},
        };
}

QVariant AbstractToolParams::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= params_.size())
        return QVariant();
    switch (role) {
    case ParamRole::ParamNameRole:

        break;
    default:
        break;
    }

    return QVariant();
}

bool AbstractToolParams::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

} // namespace quicktools
