#pragma once

#include "CoreGlobal.h"

#include <QObject>
#include <QStringList>

#undef slots
#include <pybind11/embed.h>
#define slots Q_SLOTS

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractPythonInterface : public QObject
{
    Q_OBJECT
public:
    AbstractPythonInterface(QObject *parent = nullptr);

    /**
     * @brief 析构, 释放 @ref module 和 @ref obj 的资源
     */
    virtual ~AbstractPythonInterface();

    /**
     * @brief 初始化, 导入 @ref importModule 中定义的模块
     * @return
     */
    std::tuple<int, QString> init();

    /**
     * @brief 重新加载 @ref importModule 中定义的模块 , 刷新 python 代码
     * @return
     */
    std::tuple<int, QString> reloadModule();

    /**
     * @brief 释放 @ref module 和 @ref obj 的资源
     */
    void release();

    /**
     * @brief 用于模块导入后持久化, 在接口类释放时释放资源
     */
    pybind11::module_ module;

    /**
     * @brief 可以用于模块内定义的类实例持久化, 在接口类释放时释放资源
     */
    pybind11::object obj;

protected:
    /**
     * @brief 导入的 python 模块
     * @return
     */
    virtual QString importModule() const = 0;
};

} // namespace quicktools::core
