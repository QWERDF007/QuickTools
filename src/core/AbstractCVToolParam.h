#pragma once

#include "AbstractQuickToolParam.h"
#include "CVToolROI.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVInputParams : public AbstractInputParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVInputParams)
    QML_UNCREATABLE("Can't not create a AbstractCVInputParams directly")
    Q_PROPERTY(CVToolROI *roi READ roi CONSTANT FINAL)
public:
    AbstractCVInputParams(QObject *parent = nullptr);

    virtual ~AbstractCVInputParams() {}

    QString name() const override
    {
        return "AbstractCVInputParams";
    }

    CVToolROI *roi();

private:
    CVToolROI *roi_{nullptr};
};

class QUICKTOOLS_CORE_EXPORT AbstractCVOutputParams : public AbstractOutputParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVOutputParams)
    QML_UNCREATABLE("Can't not create a AbstractCVOutputParams directly")
public:
    AbstractCVOutputParams(QObject *parent = nullptr);

    virtual ~AbstractCVOutputParams() {}

    QString name() const override
    {
        return "AbstractCVOutputParams";
    }
};

} // namespace quicktools::core
