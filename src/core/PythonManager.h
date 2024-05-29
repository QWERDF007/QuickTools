#pragma once

#include "Singleton.h"

#include <QObject>
#include <QStringList>

namespace quicktools::core {
class PythonManager : public QObject
{
    Q_OBJECT
public:
    SINGLETON(PythonManager)

    int  init();
    bool isInit() const;

    QString defaultPythonHome() const;
    QString pythonHome() const;
    bool    setPythonHome(const QString &python_home);

    QStringList sysPaths() const;
    void        addSysPaths(const QStringList &sys_paths);

public slots:

private:
    explicit PythonManager(QObject *parent = nullptr);
    ~PythonManager();

    QStringList sys_paths_;
    QString     python_home_;

    bool is_init_{false};
};
} // namespace quicktools::core
