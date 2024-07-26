#pragma once

#include "QuickTool.h"
#include "QuickToolSettings.h"

#undef slots
#include <pybind11/embed.h>
#define slots Q_SLOTS

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractPythonInterface
{
public:
    AbstractPythonInterface();
    virtual ~AbstractPythonInterface();

    int init();
    std::tuple<int, QString> reloadModule();

protected:
    virtual QString importModule() const = 0;

    pybind11::module_ module_;
};

class QUICKTOOLS_CORE_EXPORT AbstractPythonTool
    : public AbstractTool<InputParams, OutputParams, QuickToolSettings>
    , public AbstractPythonInterface
{
    Q_OBJECT
public:
    AbstractPythonTool(QObject *parent = nullptr)
        : AbstractTool(parent)
        , AbstractPythonInterface()
    {
    }

    virtual ~AbstractPythonTool() {}

    Q_INVOKABLE void reloadModule();

    bool hasPython() const override;

protected:
    int doInInit() override;
};

inline bool AbstractPythonTool::hasPython() const
{
    return true;
}

inline int AbstractPythonTool::doInInit()
{
    return AbstractPythonInterface::init();
}

} // namespace quicktools::core
