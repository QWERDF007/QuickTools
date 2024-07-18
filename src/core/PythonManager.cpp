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
    connect(this, &PythonManager::pythonHomeChanged, this, &PythonManager::initializeInterpreter);
}

PythonManager::~PythonManager()
{
    finalizeInterpreter();
}

int PythonManager::init()
{
    bool ok = setPythonHome(DefaultPythonHome());
    return ok ? 0 : -1;
}

bool PythonManager::isInit() const
{
    return Py_IsInitialized();
}

int PythonManager::initializeInterpreter()
{
    try
    {
        //        qInfo() << __FUNCTION__ << __LINE__ << "The GIL state is" << PyGILState_Check();
        if (!QFile::exists(GetPythonExecutable(python_home_)))
            return -1;
        finalizeInterpreter();
#if (PY_MAJOR_VERSION == 3) && (PY_MINOR_VERSION < 11)
        Py_SetPythonHome(Py_DecodeLocale(python_home_.toLocal8Bit().constData(), nullptr));
        pybind11::initialize_interpreter();
#else
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        PyConfig_SetBytesString(&config, &config.home, python_home_.toLocal8Bit().constData());
        pybind11::initialize_interpreter(&config);
        PyConfig_Clear(&config);
#endif
        {
            pybind11::object sys = pybind11::module_::import("sys");
            //            qInfo() << __FUNCTION__ << __LINE__ << getPythonVersion(python_home_);
            sys.attr("path").attr("append")(DefaultPythonCodeHome().toLocal8Bit().toStdString());
            //        pybind11::exec("import sys;print(sys.path, flush=True);");
        }
        // 释放 gil
        gil_release_ = new pybind11::gil_scoped_release();
        return 0;
    }
    catch (const pybind11::error_already_set &e)
    {
        spdlog::error("Failed to set initialize interpreter: {}", e.what());
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to set initialize interpreter: {}", e.what());
    }
    catch (...)
    {
        spdlog::error("Failed to set initialize interpreter with unknown exception!");
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
    }
}

QString PythonManager::DefaultPythonHome()
{
    //    QString python_home = QDir::homePath() + QDir::separator() + "test";
    QString python_home = "D:/Software/anaconda3/envs/test2";
    return python_home;
}

QString PythonManager::DefaultPythonCodeHome()
{
    return QDir::cleanPath(QDir::currentPath() + "/py_module");
}

QString PythonManager::pythonHome() const
{
    return python_home_;
}

bool PythonManager::setPythonHome(const QString &python_home)
{
    spdlog::info("设置 PYTHON_HOME: {}", python_home.toUtf8().constData());
    if (python_home_ == python_home)
        return true;
    if (GetPythonExecutable(python_home).isEmpty())
        return false;
    python_home_ = python_home;
    emit pythonHomeChanged();
    return true;
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

QString PythonManager::getPythonVersion(const QString &python_home)
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

} // namespace quicktools::core
