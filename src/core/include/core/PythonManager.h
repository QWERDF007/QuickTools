#pragma once

#include "CoreGlobal.h"
#include "common/Singleton.h"

#undef slots
#include <pybind11/embed.h>
#define slots Q_SLOTS

#include <QObject>
#include <QStringList>

namespace quicktools::core {

/**
 * @brief Python 环境管理, 负责初始化/切换/释放 python 解释器
 */
class QUICKTOOLS_CORE_EXPORT PythonManager : public QObject
{
    Q_OBJECT
public:
    SINGLETON(PythonManager)

    /**
     * @brief 初始化 python 环境
     * @return
     */
    int init();

    /**
     * @brief python 环境是否初始化
     * @return
     */
    bool isInit() const;

    /**
     * @brief 初始化 python 解释器, 这之后才能调用 python 相关
     * @param python_home[in] python 环境目录
     * @return
     */
    int initializeInterpreter(const QString &python_home);

    /**
     * @brief 获取 python 全局锁, 释放 python 解释器, 这之后不能再调用 python 相关
     */
    void finalizeInterpreter();

    /**
     * @brief 获取当前的 python 环境
     * @return 当前的 python 环境
     */
    QString pythonHome() const;

    /**
     * @brief 设置 python 环境
     * @param[in] python_home python 环境目录
     */
    void setPythonHome(const QString &python_home);

public:

    /**
     * @brief 默认的 python 环境
     * @return
     */
    static QString DefaultPythonHome();

    /**
     * @brief 导入 python 代码的目录
     * @return
     */
    static QString DefaultPythonCodeHome();

    /**
     * @brief 获取 python 可执行文件的路径
     * @param[in] python_home python 环境目录
     * @return
     */
    static QString GetPythonExecutable(const QString &python_home);

    /**
     * @brief 获取指定 python 环境的 python 版本
     * @param[in] python_home python 环境目录
     * @return python 版本
     */
    static QString GetPythonVersion(const QString &python_home);

    /**
     * @brief 判断 python 环境是否有效
     * @param[in] python_home python 环境目录
     * @return
     */
    static bool IsPythonHomeValid(const QString &python_home);

private:
    explicit PythonManager(QObject *parent = nullptr);
    ~PythonManager();

    /// 当前 python 环境
    QString python_home_;

    /// 实例化后会在当前作用域内释放释放 python 全局锁, 即在主线程释放 gil, 直到此实例析构
    pybind11::gil_scoped_release *gil_release_{nullptr};

signals:

    /**
     * @brief python 环境改变
     */
    void pythonHomeChange(const QString &);

    /**
     * @brief python 环境已经改变
     */
    void pythonHomeChanged();
};

inline int PythonManager::init()
{
    setPythonHome(DefaultPythonHome());
    return isInit();
}

inline bool PythonManager::isInit() const
{
    return Py_IsInitialized();
}

inline QString PythonManager::pythonHome() const
{
    return python_home_;
}


} // namespace quicktools::core
