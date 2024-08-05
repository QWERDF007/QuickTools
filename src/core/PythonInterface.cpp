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
    if (module)
    {
        pybind11::gil_scoped_acquire acquire;
        module.release();
    }
}

std::tuple<int, QString> AbstractPythonInterface::init()
{
    int     ret = 0;
    QString msg{"初始化成功"};
    if (!PythonManager::getInstance()->isInit())
        return {ret, "Python 环境未初始化"};
    try
    {
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

} // namespace quicktools::core
