#pragma once
#include "core/PythonTool.h"
#include "core/QuickToolManager.h"
#include "core/QuickToolType.h"

namespace quicktools::samples {

class PyTest : public core::AbstractPythonTool
{
public:
    PyTest(QObject *parent = nullptr);
    ~PyTest() = default;

    QString name() const override
    {
        return "PyTest";
    }

    std::tuple<int, QString> doInProcess() override;

    QString doc() const override
    {
        return "No Doc";
    }

protected:
    int initInputParams() override;
    int initOutputParams() override;
    int initSettings() override;

    QString importModule() const override;

private:
};

REGISTER_QUICKTOOL(core::quicktooltype::PyTestTool, PyTest)
} // namespace quicktools::samples
