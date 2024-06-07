#include "core/CVTool.h"

namespace quicktools::core {

AbstractCVTool::AbstractCVTool(QObject *parent)
    : AbstractTool(parent)
{
}

AbstractCVTool::~AbstractCVTool() {}

AbstractPythonCVTool::AbstractPythonCVTool(QObject *parent)
    : AbstractCVTool(parent)
    , AbstractPythonInterface()
{
}

AbstractPythonCVTool::~AbstractPythonCVTool()
{
    qInfo() << __FUNCTION__ << this;
}

void AbstractPythonCVTool::reloadModule()
{
    const auto [status, msg] = AbstractPythonInterface::reloadModule();
    emit showMessage(status == 0 ? InfoLevel::Success : InfoLevel::Error, msg);
}

int AbstractPythonCVTool::doInInit()
{
    return AbstractPythonInterface::init();
}

} // namespace quicktools::core
