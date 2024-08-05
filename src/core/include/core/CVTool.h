#pragma once

#include "CVToolParams.h"
#include "CVToolSettings.h"
#include "QuickTool.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVTool : public AbstractTool<CVInputParams, CVOutputParams, CVToolSettings>
{
public:
    AbstractCVTool(QObject *parent = nullptr);

    virtual ~AbstractCVTool();
};

} // namespace quicktools::core
