#pragma once

#include <QObject>
#include <QAbstractListModel>

namespace quicktools {

class AbstractQuickTool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* params READ params CONSTANT)
public:
    AbstractQuickTool(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    virtual ~AbstractQuickTool() = default;

    virtual QAbstractListModel* params() = 0;

    int operator()()
    {
        return run();
    }

    virtual int run() = 0;

private:
    Q_DISABLE_COPY(AbstractQuickTool)
};

class AbstractToolParams : QAbstractListModel
{
public:
    AbstractToolParams(QObject *parent = nullptr) : QAbstractListModel(parent) {}
    ~AbstractToolParams() = default;

    enum ParamRole
    {
        ParamNameRole = Qt::UserRole + 1,
        ParamTypeRole,
        ParamValueRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        if (parent.isValid())
            return 0;
        return params_.size();
    }

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

protected:
    QVector<QMap<int, QVariant>> params_;
};

} // namespace quicktools
