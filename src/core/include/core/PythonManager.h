#pragma once

#include "CoreGlobal.h"
#include "common/Singleton.h"

#undef slots
#include <pybind11/embed.h>
#define slots Q_SLOTS

#include <QObject>
#include <QStringList>

namespace quicktools::core {
class QUICKTOOLS_CORE_EXPORT PythonManager : public QObject
{
    Q_OBJECT
public:
    SINGLETON(PythonManager)

    int  init();
    bool isInit() const;
    int  initializeInterpreter();
    void finalizeInterpreter();

    static QString DefaultPythonHome();
    static QString DefaultPythonCodeHome();
    static QString GetPythonExecutable(const QString &python_home);

    QString pythonHome() const;
    bool    setPythonHome(const QString &python_home);

    QString getPythonVersion(const QString &python_home);

private:
    explicit PythonManager(QObject *parent = nullptr);
    ~PythonManager();

    void releaseGil();

    QString                       python_home_;
    pybind11::gil_scoped_release *gil_release_{nullptr};
signals:
    void pythonHomeChanged();
};
} // namespace quicktools::core
