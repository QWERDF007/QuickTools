#include "imagehistogram.h"

namespace quicktools {

namespace {

enum ParamRole {
    NameRole =  Qt::UserRole + 1,
};
}

ImageHistogram::ImageHistogram(QObject *parent) : AbstractQuickTool(parent) {}

QAbstractListModel *ImageHistogram::params()
{
    return params_;
}

int ImageHistogram::run()
{
    return 0;
}

int ImageHistogram::ImageHistogramParams::rowCount(const QModelIndex &parent) const
{
    return 0;
}

QHash<int, QByteArray> ImageHistogram::ImageHistogramParams::roleNames() const
{
    return {
        {NameRole, "name"},
    };
}

QVariant ImageHistogram::ImageHistogramParams::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

} // namespace quicktools
