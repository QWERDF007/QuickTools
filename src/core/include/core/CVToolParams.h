#pragma once

#include "CVToolShape.h"
#include "QuickToolParams.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT CVInputParams : public InputParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVInputParams)
    QML_UNCREATABLE("Can't not create a CVInputParams directly")
    Q_PROPERTY(CVToolROI *roi READ roi CONSTANT FINAL)
public:
    CVInputParams(QObject *parent = nullptr, QQmlEngine *qml_engine = nullptr, QJSEngine *js_engine = nullptr);

    virtual ~CVInputParams() {}

    QString name() const override
    {
        return "CVInputParams";
    }

    CVToolROI *roi();

private:
    CVToolROI *roi_{nullptr};
};

class QUICKTOOLS_CORE_EXPORT CVOutputParams : public OutputParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVOutputParams)
    QML_UNCREATABLE("Can't not create a CVOutputParams directly")
public:
    CVOutputParams(QObject *parent = nullptr, QQmlEngine *qml_engine = nullptr, QJSEngine *js_engine = nullptr);

    virtual ~CVOutputParams() {}

    QString name() const override
    {
        return "CVOutputParams";
    }

    CVToolShapeListModel *shapesListModel()
    {
        return shapes_list_model_;
    }

private:
    CVToolShapeListModel *shapes_list_model_;
};

} // namespace quicktools::core
