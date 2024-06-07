#pragma once

#include "CVToolParams.h"
#include "CVToolSettings.h"
#include "PythonTool.h"
#include "QuickTool.h"


namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVTool : public AbstractTool<CVInputParams, CVOutputParams, CVToolSettings>
{
public:
    AbstractCVTool(QObject *parent = nullptr);

    virtual ~AbstractCVTool();
};

class QUICKTOOLS_CORE_EXPORT AbstractPythonCVTool
    : public AbstractCVTool
    , public AbstractPythonInterface
{
    Q_OBJECT
    Q_PROPERTY(bool hasPython READ hasPython CONSTANT FINAL)
public:
    AbstractPythonCVTool(QObject *parent = nullptr);

    virtual ~AbstractPythonCVTool();

    Q_INVOKABLE void reloadModule();

protected:
    int doInInit() override;
};

} // namespace quicktools::core
