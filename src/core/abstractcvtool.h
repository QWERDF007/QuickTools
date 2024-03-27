#pragma once

#include "AbstractCVToolParam.h"
#include "AbstractQuickTool.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVTool : public AbstractTool<AbstractCVInputParams, AbstractCVOutputParams>
{
public:
    AbstractCVTool(QObject *parent = nullptr)
        : AbstractTool(parent)
    {
    }

    virtual ~AbstractCVTool() {}
};

} // namespace quicktools::core
