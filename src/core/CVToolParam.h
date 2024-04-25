#pragma once

#include "AbstractQuickToolParam.h"
#include "CVToolROI.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT CVInputParams : public AbstractInputParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVInputParams)
    QML_UNCREATABLE("Can't not create a CVInputParams directly")
    Q_PROPERTY(CVToolROI *roi READ roi CONSTANT FINAL)
public:
    CVInputParams(QObject *parent = nullptr);

    virtual ~CVInputParams() {}

    QString name() const override
    {
        return "CVInputParams";
    }

    CVToolROI *roi();

private:
    CVToolROI *roi_{nullptr};
};

class QUICKTOOLS_CORE_EXPORT CVOutputParams : public AbstractOutputParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVOutputParams)
    QML_UNCREATABLE("Can't not create a CVOutputParams directly")
public:
    CVOutputParams(QObject *parent = nullptr);

    virtual ~CVOutputParams() {}

    QString name() const override
    {
        return "CVOutputParams";
    }
};

} // namespace quicktools::core
