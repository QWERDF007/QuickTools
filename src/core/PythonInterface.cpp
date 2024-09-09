#include "core/PythonInterface.h"

#include "core/PythonManager.h"

#include <spdlog/spdlog.h>

namespace quicktools::core {

AbstractPythonInterface::AbstractPythonInterface(QObject *parent)
    : QObject(parent)
{
}

AbstractPythonInterface::~AbstractPythonInterface()
{
    release();
}

std::tuple<int, QString> AbstractPythonInterface::init()
{
    int     ret = 0;
    QString msg{"初始化成功"};
    if (!PythonManager::getInstance()->isInit())
        return {-1, "Python 环境未初始化"};
    try
    {
        if (importModule().isEmpty())
            return {ret, "模块为空"};
        pybind11::gil_scoped_acquire acquire;
        module = pybind11::module_::import(importModule().toLocal8Bit().constData());
        spdlog::debug("导入 python 模块: {}", importModule().toUtf8().constData());
    }
    catch (const std::exception &e)
    {
        ret = -1;
        msg = e.what();
        spdlog::error("导入 python 模块: {}, 失败: {}", importModule().toUtf8().constData(), e.what());
    }
    return {ret, msg};
}

std::tuple<int, QString> AbstractPythonInterface::reloadModule()
{
    int     ret = 0;
    QString msg = "重新加载成功";
    try
    {
        if (module)
        {
            pybind11::gil_scoped_acquire acquire;
            module.reload();
            if (obj)
                obj.release();
        }
        else
        {
            ret = -1;
            msg = "module 为空";
        }
    }
    catch (const std::exception &e)
    {
        ret = -1;
        msg = e.what();
        spdlog::error("重新加载模块: {}, 失败: {}", importModule().toUtf8().constData(), e.what());
    }
    return {ret, msg};
}

void AbstractPythonInterface::release()
{
    if (!Py_IsInitialized())
        return;
    pybind11::gil_scoped_acquire acquire;
    {
        if (module)
            module.release();
        if (obj)
            obj.release();
    }
}

} // namespace quicktools::core
