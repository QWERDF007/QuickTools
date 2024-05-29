#include "AbstractCVTool.h"

namespace quicktools::core {

AbstractCVTool::AbstractCVTool(QObject *parent)
    : AbstractTool(parent)
{
}

AbstractCVTool::~AbstractCVTool() {}

AbstractPythonCVTool::AbstractPythonCVTool(QObject *parent) : AbstractCVTool(parent), AbstractPythonTool()
{

}

AbstractPythonCVTool::~AbstractPythonCVTool()
{
    qInfo() << __FUNCTION__ << this;
}

int AbstractPythonCVTool::doInInit()
{
    return AbstractPythonTool::init();
}



} // namespace quicktools::core
