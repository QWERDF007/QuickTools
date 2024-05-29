#include "AbstractPythonTool.h"

#include "PythonManager.h"

#include <QDebug>

namespace quicktools::core {

AbstractPythonTool::AbstractPythonTool() {}

AbstractPythonTool::~AbstractPythonTool()
{
    qInfo() << __FUNCTION__ << this;
}

int AbstractPythonTool::init()
{
    qInfo() << __FUNCTION__;
    int ret = 0;
    if (!PythonManager::getInstance()->isInit())
    {
        ret = PythonManager::getInstance()->init();
    }
    if (ret == 0)
    {
        PythonManager::getInstance()->addSysPaths(importPaths());
    }
    return ret;
}

QStringList AbstractPythonTool::importPaths()
{
    return QStringList();
}

} // namespace quicktools::core
