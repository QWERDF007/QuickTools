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

    int                      init();
    std::tuple<int, QString> reloadModule();

    bool hasPython() const
    {
        return true;
    }

protected:
    virtual QString importModule() const = 0;

    pybind11::module_ module_;
};

class QUICKTOOLS_CORE_EXPORT AbstractPythonTool
    : public AbstractTool<InputParams, OutputParams, QuickToolSettings>
    , public AbstractPythonInterface
{
    Q_OBJECT
    Q_PROPERTY(bool hasPython READ hasPython CONSTANT FINAL)
public:
    AbstractPythonTool(QObject *parent = nullptr)
        : AbstractTool(parent)
        , AbstractPythonInterface()
    {
    }

    virtual ~AbstractPythonTool() {}

    Q_INVOKABLE void reloadModule();

protected:
    int doInInit() override;
};

} // namespace quicktools::core
