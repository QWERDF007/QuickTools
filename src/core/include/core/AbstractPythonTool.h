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

    int                      init();
    std::tuple<int, QString> reloadModule();

protected:
    virtual QString importModule() const = 0;

    pybind11::module_ module_;
};

} // namespace quicktools::core
