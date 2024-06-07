#include "core/PythonTool.h"

#include "core/PythonManager.h"

#include <QDebug>
#include <QString>
#include <QThread>

namespace quicktools::core {

AbstractPythonInterface::AbstractPythonInterface() {}

AbstractPythonInterface::~AbstractPythonInterface()
{
    qInfo() << __FUNCTION__ << this;
    pybind11::gil_scoped_acquire acquire;
    if (module_)
        module_.release();
}

int AbstractPythonInterface::init()
{
    qInfo() << __FUNCTION__;
    int ret = 0;
    if (!PythonManager::getInstance()->isInit())
        ret = PythonManager::getInstance()->init();
    if (ret != 0)
        return ret;
    try
    {
        pybind11::gil_scoped_acquire acquire;
        qInfo() << __FUNCTION__ << __LINE__ << "import module:" << importModule();
        module_ = pybind11::module_::import(importModule().toLocal8Bit().constData());
        return 0;
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
    return -1;
}

std::tuple<int, QString> AbstractPythonInterface::reloadModule()
{
    pybind11::gil_scoped_acquire acquire;

    int     status = 0;
    QString msg    = QObject::tr("刷新Python模块成功");
    try
    {
        if (module_)
            module_.reload();
    }
    catch (const pybind11::error_already_set &e)
    {
        status = -1;
        msg    = e.what();
    }
    catch (const std::exception &e)
    {
        status = -1;
        msg    = e.what();
    }
    return {status, msg};
}

void AbstractPythonTool::reloadModule()
{
    const auto [status, msg] = AbstractPythonInterface::reloadModule();
    emit showMessage(status == 0 ? InfoLevel::Success : InfoLevel::Error, msg);
}

int AbstractPythonTool::doInInit()
{
    return AbstractPythonInterface::init();
}

} // namespace quicktools::core
