#pragma once

#include "AbstractQuickTool.h"
#include "CVToolParams.h"
#include "CVToolSettings.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVTool : public AbstractTool<CVInputParams, CVOutputParams, CVToolSettings>
{
public:
    AbstractCVTool(QObject *parent = nullptr)
        : AbstractTool(parent)
    {
    }

    virtual ~AbstractCVTool() {}
};

} // namespace quicktools::core
