#include "PythonManager.h"

#include "Utils.h"

#include <QDebug>
#include <QDir>


#undef slots
#include "pybind11/embed.h"
#define slots Q_SLOTS

namespace quicktools::core {

PythonManager::PythonManager(QObject *parent)
    : QObject(parent)
    , is_init_{false}
{
    qInfo() << __FUNCTION__ << this;
    setPythonHome(defaultPythonHome());
}

PythonManager::~PythonManager()
{
    qInfo() << __FUNCTION__ << this;
}

int PythonManager::init()
{
    return 0;
}

bool PythonManager::isInit() const
{
    return is_init_;
}

QString PythonManager::defaultPythonHome() const
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
    if (python_home_ == python_home)
        return false;
#if defined(_WIN32)
    QString python_executable = QDir::cleanPath(python_home) + "/python.exe";
#else
    QString python_executable = QDir::cleanPath(python_home) + "/python";
#endif
    if (!QFile::exists(python_executable))
        return false;
    // qputenv("PYTHONHOME", python_home.toLocal8Bit());
    try
    {
#if (PY_MAJOR_VERSION == 3) && (PY_MINOR_VERSION < 11)
        Py_SetPythonHome(Py_DecodeLocale(python_home.toLocal8Bit().constData(), nullptr));
        pybind11::initialize_interpreter()
#else
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        PyConfig_SetBytesString(&config, &config.home, python_home.toLocal8Bit().constData());
        pybind11::initialize_interpreter(&config);
        PyConfig_Clear(&config);
#endif
            python_home_
            = python_home;
        return true;
    }
    catch (const pybind11::error_already_set &e)
    {
        qCritical() << __FUNCTION__ << "failed to set PYTHONHOME:" << e.what();
    }
    catch (const std::exception &e)
    {
        qCritical() << __FUNCTION__ << "failed to set PYTHONHOME:" << e.what();
    }
    return false;
}

QStringList PythonManager::sysPaths() const
{
    return sys_paths_;
}

void PythonManager::addSysPaths(const QStringList &sys_paths)
{
    for (const QString &path : sys_paths)
    {
        if (sys_paths_.contains(path))
            continue;
        sys_paths_.append(path);
    }
}

} // namespace quicktools::core
