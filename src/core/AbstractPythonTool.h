#pragma once

#include <QObject>
#include <QString>
#include <QStringList>

#undef slots
#include <pybind11/embed.h>
#define slots Q_SLOTS

namespace quicktools::core {

class AbstractPythonTool
{
public:
    AbstractPythonTool();
    virtual ~AbstractPythonTool();

    int init();

    virtual QStringList importPaths();
    virtual QString     importModule() = 0;

protected:
    pybind11::object object_;
};

} // namespace quicktools::core
