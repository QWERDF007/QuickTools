#include "PythonManager.h"

#include <QDebug>
#include <QDir>

namespace quicktools::core {

PythonManager::PythonManager(QObject *parent)
    : QObject(parent)
{
    qInfo() << __FUNCTION__ << this;
    connect(this, &PythonManager::pythonHomeChanged, this, &PythonManager::initializeInterpreter);
    setPythonHome(defaultPythonHome());
}

PythonManager::~PythonManager()
{
    qInfo() << __FUNCTION__ << this;
    finalizeInterpreter();
}

int PythonManager::init()
{
    return 0;
}

bool PythonManager::isInit() const
{
    return Py_IsInitialized();
}

int PythonManager::initializeInterpreter()
{
    try
    {
        if (!QFile::exists(getPythonExecutable(python_home_)))
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
        sys_ = pybind11::module_::import("sys");
        return 0;
    }
    catch (const pybind11::error_already_set &e)
    {
        qCritical() << __FUNCTION__ << __LINE__ << "failed to set initialize interpreter:" << e.what();
    }
    catch (const std::exception &e)
    {
        qCritical() << __FUNCTION__ << __LINE__ << "failed to set initialize interpreter:" << e.what();
    }
    catch (...)
    {
        qCritical() << __FUNCTION__ << __LINE__ << "failed to set initialize interpreter with unknown exception.";
    }
    return -1;
}

void PythonManager::finalizeInterpreter()
{
    if (Py_IsInitialized())
    {
        pybind11::finalize_interpreter();
    }
}

QString PythonManager::defaultPythonHome()
{
    QString python_home = QDir::homePath() + QDir::separator() + "test";
    return python_home;
}

QString PythonManager::pythonHome() const
{
    return python_home_;
}

bool PythonManager::setPythonHome(const QString &python_home)
{
    if (python_home_ == python_home || getPythonExecutable(python_home).isEmpty())
        return false;
    python_home_ = python_home;
    emit pythonHomeChanged();
    return true;
}

QString PythonManager::getPythonExecutable(const QString &python_home)
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

QStringList PythonManager::sysPaths() const
{
    return sys_paths_;
}

void PythonManager::addSysPaths(const QStringList &sys_paths)
{
    if (!isInit() || sys_paths.empty())
        return;
    for (const QString &path : sys_paths)
    {
        if (sys_paths_.contains(path))
            continue;
        sys_paths_.append(path);
    }
}

} // namespace quicktools::core
