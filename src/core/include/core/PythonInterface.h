#pragma once

#include "CoreGlobal.h"

#include <QObject>
#include <QStringList>

#undef slots
#include <pybind11/embed.h>
#define slots Q_SLOTS

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractPythonInterface : public QObject
{
    Q_OBJECT
public:
    AbstractPythonInterface(QObject *parent = nullptr);
    virtual ~AbstractPythonInterface();

    std::tuple<int, QString> init();

    std::tuple<int, QString> reloadModule();

    pybind11::module_ module;

protected:
    virtual QString importModule() const = 0;
};

} // namespace quicktools::core
