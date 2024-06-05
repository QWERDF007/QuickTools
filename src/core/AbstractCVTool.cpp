#include "core/AbstractCVTool.h"

namespace quicktools::core {

AbstractCVTool::AbstractCVTool(QObject *parent)
    : AbstractTool(parent)
{
}

AbstractCVTool::~AbstractCVTool() {}

AbstractPythonCVTool::AbstractPythonCVTool(QObject *parent)
    : AbstractCVTool(parent)
    , AbstractPythonTool()
{
}

AbstractPythonCVTool::~AbstractPythonCVTool()
{
    qInfo() << __FUNCTION__ << this;
}

void AbstractPythonCVTool::reloadModule()
{
    const auto [status, msg] = AbstractPythonTool::reloadModule();
    emit showMessage(status == 0 ? InfoLevel::Success : InfoLevel::Error, msg);
}

int AbstractPythonCVTool::doInInit()
{
    return AbstractPythonTool::init();
}

} // namespace quicktools::core
