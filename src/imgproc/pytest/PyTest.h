#pragma once
#include "AbstractCVTool.h"
#include "QuickToolType.h"

namespace quicktools::imgproc {

class PyTest : public core::AbstractPythonCVTool
{
public:
    PyTest(QObject* parent=nullptr);
    ~PyTest() = default;
    QString name() const override
    {
        return "PyTest";
    }

    std::tuple<int, QString> process() override;

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
} // namespace quicktools::imgproc
