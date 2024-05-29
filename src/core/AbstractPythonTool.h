#pragma once

#include <QObject>
#include <QString>
#include <QStringList>

namespace quicktools::core {

class AbstractPythonTool
{
public:
    AbstractPythonTool();
    virtual ~AbstractPythonTool();

    int init();

    virtual QStringList importPaths();
    virtual QString     importModule() = 0;
};

} // namespace quicktools::core
