#pragma once

#include "CVToolParams.h"
#include "CVToolSettings.h"
#include "ImageProvider.h"
#include "QuickTool.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVTool : public AbstractTool<CVInputParams, CVOutputParams, CVToolSettings>
{
    Q_OBJECT
    Q_PROPERTY(ImageProviderList *providersList READ providersList CONSTANT FINAL)
    Q_PROPERTY(int acceptedShapes READ acceptedShapes CONSTANT FINAL)
public:
    AbstractCVTool(QObject *parent = nullptr, QQmlEngine *qml_engine = nullptr, QJSEngine *js_engine = nullptr);

    virtual ~AbstractCVTool();

    ImageProviderList *providersList()
    {
        return providers_;
    }

    virtual int acceptedShapes() const
    {
        return CVToolShape::AllShapes;
    }

protected:
    ImageProviderList *providers_{nullptr};
};

} // namespace quicktools::core
