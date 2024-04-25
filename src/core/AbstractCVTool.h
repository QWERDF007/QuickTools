#pragma once

#include "AbstractQuickTool.h"
#include "CVToolParam.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVTool : public AbstractTool<CVInputParams, CVOutputParams>
{
public:
    AbstractCVTool(QObject *parent = nullptr)
        : AbstractTool(parent)
    {
    }

    virtual ~AbstractCVTool() {}
};

} // namespace quicktools::core
