#include "PythonManager.h"

#include <QDebug>
#include <QDir>


namespace quicktools::core {

PythonManager::PythonManager()
{
    qInfo() << __FUNCTION__ << this;
    setPythonHome(defaultPythonHome());
}

PythonManager::~PythonManager()
{
    qInfo() << __FUNCTION__ << this;
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
    python_home_ = python_home;
    return true;
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
