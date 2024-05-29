#include "AbstractPythonTool.h"

#include "PythonManager.h"

#include <QDebug>


namespace quicktools::core {

AbstractPythonTool::AbstractPythonTool()
{
}

AbstractPythonTool::~AbstractPythonTool()
{
    qInfo() << __FUNCTION__ << this;
}

int AbstractPythonTool::init()
{
    qInfo() << __FUNCTION__;
    if (!PythonManager::getInstance()->isInit())
    {
        return -1;
    }
    PythonManager::getInstance()->addSysPaths(importPaths());
    return 0;
}

QStringList AbstractPythonTool::importPaths()
{
    return QStringList();
}

} // namespace quicktools::core
