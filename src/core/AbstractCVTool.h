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
    Q_OBJECT
    Q_PROPERTY(bool hasPython READ hasPython CONSTANT FINAL)
public:
    AbstractPythonCVTool(QObject *parent = nullptr);

    virtual ~AbstractPythonCVTool();

    bool hasPython() const
    {
        return true;
    }

    Q_INVOKABLE void reloadModule();

protected:
    int doInInit() override;
};

} // namespace quicktools::core
