#pragma once

#include <QAbstractListModel>
#include <QObject>

#include "quicktoolstypes_global.h"

namespace quicktools {


class QUICKTOOLS_TYPES_EXPORT AbstractToolParams : public QAbstractListModel
{
    Q_OBJECT
public:
    AbstractToolParams(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
    }

    virtual ~AbstractToolParams() {}

    enum ToolParamRole
    {
        ParamNameRole = Qt::UserRole + 1,
        ParamTypeRole,
        ParamVisibleRole,
        ParamValueRole,
        ParamRangeRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        if (parent.isValid())
            return 0;
        return params_.size();
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            {   ParamNameRole,    "name"},
            {   ParamTypeRole,    "type"},
            {ParamVisibleRole, "visible"},
            {  ParamValueRole,   "value"},
            {  ParamRangeRole,   "range"},
        };
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

protected:
    QVector<QMap<int, QVariant>> params_;
};

class QUICKTOOLS_TYPES_EXPORT AbstractToolInputParams : public AbstractToolParams
{
    Q_OBJECT
public:
    AbstractToolInputParams(QObject *parent = nullptr)
        : AbstractToolParams(parent)
    {
    }

    virtual ~AbstractToolInputParams() {}
};

class QUICKTOOLS_TYPES_EXPORT AbstractToolOutputParams : public AbstractToolParams
{
    Q_OBJECT
public:
    AbstractToolOutputParams(QObject *parent = nullptr)
        : AbstractToolParams(parent)
    {
    }

    virtual ~AbstractToolOutputParams() {}
};

class QUICKTOOLS_TYPES_EXPORT AbstractQuickTool : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QAbstractListModel* inputParams READ inputParams NOTIFY inputParamsChanged FINAL)
    Q_PROPERTY(QAbstractListModel* outputParams READ outputParams NOTIFY outputParamsChanged FINAL)
public:
    AbstractQuickTool(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    virtual ~AbstractQuickTool() {}

    int operator()()
    {
        return run();
    }

    virtual int run() = 0;

    QAbstractListModel* inputParams() const
    {
        return input_params_;
    }

    bool setInputParams(AbstractToolInputParams* input_params);

    QAbstractListModel* outputParams() const
    {
        return output_params_;
    }

    bool setOutputParams(AbstractToolOutputParams* output_params);

private:
    Q_DISABLE_COPY(AbstractQuickTool)
    AbstractToolInputParams *input_params_{nullptr};
    AbstractToolOutputParams *output_params_{nullptr};

signals:
    void inputParamsChanged();
    void outputParamsChanged();
};


} // namespace quicktools
