#pragma once

#include <QObject>

namespace quicktools {

class AbstractQuickTool : public QObject
{
    Q_OBJECT
public:
    AbstractQuickTool(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~AbstractQuickTool() = default;
};

} // namespace quicktools
