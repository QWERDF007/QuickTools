#include "core/PythonManager.h"

#include <spdlog/spdlog.h>

#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QRegularExpression>
#include <QThread>

namespace quicktools::core {

PythonManager::PythonManager(QObject *parent)
    : QObject(parent)
{
    connect(this, &PythonManager::pythonHomeChange, this, &PythonManager::initializeInterpreter);
}

PythonManager::~PythonManager()
{
    finalizeInterpreter();
}

void PythonManager::addEnv()
{
    char   *pval;
    size_t  len;
    errno_t err = _dupenv_s(&pval, &len, "PATH");
    if (err == 0)
    {
        QString s(pval);
        s = python_home_ + ";" + s;
        s = python_home_ + "/Library/bin;" + s;
        _putenv_s("PATH", s.toLocal8Bit().constData());
    }
}

void PythonManager::clearEnv()
{
    char   *pval;
    size_t  len;
    errno_t err = _dupenv_s(&pval, &len, "PATH");
    if (err == 0)
    {
        QString s(pval);
        s.replace(python_home_ + "/Library/bin;", "");
        s.replace(python_home_ + ";", "");
        _putenv_s("PATH", s.toLocal8Bit().constData());
    }
}

int PythonManager::initializeInterpreter(const QString &python_home)
{
    try
    {
        finalizeInterpreter();
        python_home_ = python_home;
        emit pythonHomeChanged();
        if (!IsPythonHomeValid(python_home))
        {
            spdlog::error("初始化 python 环境失败, python 环境 {} 不合法!", python_home.toUtf8().constData());
            return -1;
        }
        addEnv();
#if (PY_MAJOR_VERSION == 3) && (PY_MINOR_VERSION < 11)
        Py_SetPythonHome(Py_DecodeLocale(python_home.toLocal8Bit().constData(), nullptr));
        pybind11::initialize_interpreter();
#else
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        PyConfig_SetBytesString(&config, &config.home, python_home.toLocal8Bit().constData());
        pybind11::initialize_interpreter(&config);
        PyConfig_Clear(&config);
#endif
        {
            // https://stackoverflow.com/a/36108675
            // 避免使用 std::thread 在程序结束时 python threading 报 AssertionError, 虽然正常结束
            pybind11::module_::import("threading");
            // 将 python 代码目录添加到 sys.path
            pybind11::object sys = pybind11::module_::import("sys");
            sys.attr("path").attr("append")(DefaultPythonCodeHome().toLocal8Bit().toStdString());
            // 重要 (踩坑), 避免 c++ 调用 python 脚本中用到 sys.executable 出错
            // 此处不修改则 sys.executable 会使用 QuickTools.exe
            sys.attr("executable") = GetPythonExecutable(python_home_).toLocal8Bit().toStdString();
        }
        // 释放 gil
        gil_release_ = new pybind11::gil_scoped_release();
        spdlog::info("初始化 python 环境: {}, 版本: {}", QDir(python_home_).dirName().toUtf8().constData(),
                     GetPythonVersion(python_home_).toUtf8().constData());
        return 0;
    }
    catch (const std::exception &e)
    {
        spdlog::error("初始化 python 解释器失败: {}", e.what());
    }
    catch (...)
    {
        spdlog::error("初始化 python 解释器失败, 未知错误!");
    }
    return -1;
}

void PythonManager::finalizeInterpreter()
{
    if (gil_release_)
    {
        delete gil_release_;
        gil_release_ = nullptr;
    }
    if (Py_IsInitialized())
    {
        pybind11::finalize_interpreter();
        clearEnv();
        spdlog::info("释放 python 环境: {}, 版本: {}", python_home_.toUtf8().constData(),
                     GetPythonVersion(python_home_).toUtf8().constData());
    }
}

void PythonManager::setPythonHome(const QString &python_home)
{
    spdlog::info("设置 PYTHON_HOME: {}", python_home.toUtf8().constData());
    if (python_home_ == python_home)
        return;
    initializeInterpreter(python_home);
    //    emit pythonHomeChange(python_home);
}

QString PythonManager::DefaultPythonHome()
{
    //    QString python_home = QDir::homePath() + QDir::separator() + "test";
    QString python_home = "D:/Software/anaconda3/envs/AD";
    return python_home;
}

QString PythonManager::DefaultPythonCodeHome()
{
    return QDir::cleanPath(QDir::currentPath() + "/py_module");
}

QString PythonManager::GetPythonExecutable(const QString &python_home)
{
#ifdef _WIN32
    QString python_executable = QDir::cleanPath(python_home) + "/python.exe";
#else
    QString python_executable = QDir::cleanPath(python_home) + "/python";
#endif
    if (!QFile::exists(python_executable))
        return QString();
    return python_executable;
}

QString PythonManager::GetPythonVersion(const QString &python_home)
{
    const QString python_executable = GetPythonExecutable(python_home);
    if (!QFile::exists(python_executable))
        return "";
    QProcess p;
    p.start(python_executable, QStringList{"--version"});
    p.waitForFinished();
    const QString python_version = p.readAllStandardOutput().trimmed() /*.split(' ')[1]*/;
    return python_version;
}

bool PythonManager::IsPythonHomeValid(const QString &python_home)
{
    return QFile::exists(GetPythonExecutable(python_home));
}

} // namespace quicktools::core
