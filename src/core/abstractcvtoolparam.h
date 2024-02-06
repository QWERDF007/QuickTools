#pragma once

#include "abstractquicktoolparam.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVInputParams : public AbstractInputParams
{
public:
    AbstractCVInputParams(QObject *parent = nullptr)
        : AbstractInputParams(parent)
    {
    }

    virtual ~AbstractCVInputParams() {}

    QString name() const override
    {
        return "AbstractCVInputParams";
    }
};

class QUICKTOOLS_CORE_EXPORT AbstractCVOutputParams : public AbstractOutputParams
{
public:
    AbstractCVOutputParams(QObject *parent = nullptr)
        : AbstractOutputParams(parent)
    {
    }

    virtual ~AbstractCVOutputParams() {}

    QString name() const override
    {
        return "AbstractCVOutputParams";
    }
};

} // namespace quicktools::core
