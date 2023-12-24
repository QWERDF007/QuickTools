#pragma once

#include "abstractquicktool.h"

#include <QObject>

namespace quicktools {
class ImageHistogram : AbstractQuickTool
{
public:
    ImageHistogram(QObject *parent = nullptr);

    ~ImageHistogram() = default;

    QAbstractListModel * params() override;

    int run() override;

private:
    class ImageHistogramParams : public QAbstractListModel
    {
    public:
        ImageHistogramParams(QObject *parent = nullptr) : QAbstractListModel(parent) {}
        ~ImageHistogramParams() = default;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QHash<int, QByteArray> roleNames() const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    };

    ImageHistogramParams* params_{nullptr};
};

} // namespace quicktools

