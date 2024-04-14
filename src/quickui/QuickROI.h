#pragma once

#include <QAbstractListModel>
#include <QtQml>
#include <QMap>

class QuickROI : public QObject
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(QuickROI)
public:
    explicit QuickROI(QObject *parent = nullptr);
    QuickROI(const QuickROI& other);
    QuickROI & operator=(const QuickROI& other);

    QVariant data(int role) const;
    bool setData(int role, const QVariant& value);

private:
    QMap<int, QVariant> data_;
};

class QuickROIListModel : public QAbstractListModel
{
    Q_OBJECT
    // 声明 QML 中可用
    QML_NAMED_ELEMENT(QuickROIListModel)

public:
    explicit QuickROIListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE void addROI(double x, double y, double width, double height);
    Q_INVOKABLE void deleteROI(int index);

private:
    QList<QuickROI>  roi_list_;
};

