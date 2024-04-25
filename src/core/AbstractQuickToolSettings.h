#pragma once

#include <QAbstractListModel>
#include <QtQml>
#include "CoreGlobal.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractQuickToolSettings : public QAbstractListModel
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(QuickToolSettings)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractQuickToolSettings directly")
public:
    AbstractQuickToolSettings(QObject *parent = nullptr);

    virtual ~AbstractQuickToolSettings();

    virtual QString name() const = 0;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    QMap<QString, QMap<int, QVariant>> settings_;   // [name, [key, value]]
};

} // namespace quicktools::core
