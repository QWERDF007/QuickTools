#pragma once

#include "Singleton.h"

#include <QStringList>


namespace quicktools::core {
class PythonManager
{
public:
    SINGLETON(PythonManager)

    QString defaultPythonHome() const;
    QString pythonHome() const;
    bool    setPythonHome(const QString &python_home);

    QStringList sysPaths() const;
    void        addSysPaths(const QStringList &sys_paths);

private:
    explicit PythonManager();
    ~PythonManager();

    QStringList sys_paths_;
    QString     python_home_;
};
} // namespace quicktools::core
