#pragma once

#include "AbstractPythonTool.h"
#include "AbstractQuickTool.h"
#include "CVToolParams.h"
#include "CVToolSettings.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVTool : public AbstractTool<CVInputParams, CVOutputParams, CVToolSettings>
{
public:
    AbstractCVTool(QObject *parent = nullptr);

    virtual ~AbstractCVTool();
};

class QUICKTOOLS_CORE_EXPORT AbstractPythonCVTool
    : public AbstractCVTool
    , public AbstractPythonTool
{
public:
    AbstractPythonCVTool(QObject *parent = nullptr);
    virtual ~AbstractPythonCVTool();

protected:
    int doInInit() override;
};

} // namespace quicktools::core
