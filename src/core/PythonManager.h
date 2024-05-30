#pragma once

#include "Singleton.h"

#undef slots
#include <pybind11/embed.h>
#define slots Q_SLOTS

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
    int  initializeInterpreter();
    void finalizeInterpreter();

    static QString defaultPythonHome();
    QString        pythonHome() const;
    bool           setPythonHome(const QString &python_home);

    static QString getPythonExecutable(const QString &python_home);

    QStringList sysPaths() const;
    void        addSysPaths(const QStringList &sys_paths);

private:
    explicit PythonManager(QObject *parent = nullptr);
    ~PythonManager();

    QStringList      sys_paths_;
    QString          python_home_;
    pybind11::object sys_;
signals:
    void pythonHomeChanged();
};
} // namespace quicktools::core
