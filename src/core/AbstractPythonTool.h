#pragma once
#include <QString>
#include <QStringList>

namespace quicktools::core {

class AbstractPythonTool
{
public:
    AbstractPythonTool()          = default;
    virtual ~AbstractPythonTool() = default;

    virtual QStringList importPaths();
    virtual QString     importModule() = 0;
};

} // namespace quicktools::core
