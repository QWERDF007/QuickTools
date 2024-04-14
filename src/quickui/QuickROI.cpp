#include "QuickROI.h"

enum ROIListModelRole
{
    ROIVisible = Qt::UserRole + 1,
    ROISelected,
    ROIXRole,
    ROIYRole,
    ROIWidthRole,
    ROIHeightRole,
};


QuickROI::QuickROI(QObject *parent) : QObject(parent)
{
    data_ = QMap<int, QVariant>{
                                {ROIVisible, QVariant()},
                                {ROISelected, QVariant()},
                                {ROIXRole, QVariant()},
                                {ROIYRole, QVariant()},
                                {ROIWidthRole, QVariant()},
                                {ROIHeightRole, QVariant()},
                                };
}

QuickROI::QuickROI(const QuickROI &other) : data_{other.data_}
{

}

QuickROI &QuickROI::operator=(const QuickROI &other)
{
    data_ = other.data_;
    return *this;
}

QVariant QuickROI::data(int role) const
{
    return data_.value(role, QVariant());
}

bool QuickROI::setData(int role, const QVariant &value)
{
    if (data_.find(role) == data_.end())
        return false;
    data_[role] = value;
    return true;
}



QuickROIListModel::QuickROIListModel(QObject *parent) : QAbstractListModel{parent}
{

}

int QuickROIListModel::rowCount(const QModelIndex &parent) const
{
    return roi_list_.size();
}

QVariant QuickROIListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();
    return roi_list_[index.row()].data(role);
}

bool QuickROIListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= rowCount())
        return false;
    if (!roi_list_[index.row()].setData(role, value))
        return false;
    emit dataChanged(index, index, {role});
    return true;
}

bool QuickROIListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || row > rowCount(parent))
        return false;
    beginInsertRows(QModelIndex(), row, row + count - 1);

    roi_list_.insert(row, count, QuickROI());

    endInsertRows();
    return true;
}

bool QuickROIListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    const auto it = roi_list_.begin() + row;
    roi_list_.erase(it, it + count);

    endRemoveRows();

    return true;
}

QHash<int, QByteArray> QuickROIListModel::roleNames() const
{
    return {
            {ROIVisible, "visible"},
            {ROISelected, "selected"},
            {ROIXRole, "x"},
            {ROIYRole, "y"},
            {ROIWidthRole, "width"},
            {ROIHeightRole, "height"},
            };
}

void QuickROIListModel::addROI(double x, double y, double width, double height)
{
    int row = rowCount();
    insertRow(row);
    auto _index = index(row);
    setData(_index, true, ROIVisible);
    setData(_index, false, ROISelected);
    setData(_index, x, ROIXRole);
    setData(_index, y, ROIYRole);
    setData(_index, width, ROIWidthRole);
    setData(_index, height, ROIHeightRole);
}

void QuickROIListModel::deleteROI(int index)
{

}

