#pragma once

#include "abstractquicktool.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVTool : public AbstractQuickTool
{
    Q_OBJECT

    // 声明 QML 中可用
    QML_NAMED_ELEMENT(CVTool)
    // 声明对象不能在 QML 中创建
    QML_UNCREATABLE("Can't not create a AbstractCVTool directly")
public:
    AbstractCVTool(QObject *parent = nullptr)
        : AbstractQuickTool(parent)
    {
    }

    virtual ~AbstractCVTool() {}

private:
};

} // namespace quicktools::core
