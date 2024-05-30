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
        ret = PythonManager::getInstance()->init();
    if (ret == 0)
        PythonManager::getInstance()->addSysPaths(importPaths());
    if (ret == 0)
    {
        try
        {
            object_ = pybind11::module_::import(importModule().toLocal8Bit().constData());
        }
        catch (const pybind11::error_already_set &e)
        {
            qCritical() << __FUNCTION__ << __LINE__ << "failed to import module:" << importModule() << "\n" << e.what();
        }
        catch (const std::exception &e)
        {
            qCritical() << __FUNCTION__ << __LINE__ << "failed to import module:" << importModule() << "\n" << e.what();
        }
        catch (...)
        {
            qCritical() << __FUNCTION__ << __LINE__ << "failed to import module with unknown exception.";
        }
    }
    return ret;
}

QStringList AbstractPythonTool::importPaths()
{
    return QStringList();
}

} // namespace quicktools::core
