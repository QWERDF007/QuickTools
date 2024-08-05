#pragma once
#include "core/QuickTool.h"
#include "core/QuickToolManager.h"
#include "core/QuickToolType.h"

namespace quicktools::samples {

class PyTest : public core::AbstractTool<core::InputParams, core::OutputParams, core::QuickToolSettings>
{
public:
    PyTest(QObject *parent = nullptr);

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

private:
};

REGISTER_QUICKTOOL(core::quicktooltype::PyTestTool, PyTest)
} // namespace quicktools::samples
