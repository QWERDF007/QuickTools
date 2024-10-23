#include "core/CVToolParams.h"

namespace quicktools::core {

CVInputParams::CVInputParams(QObject *parent, QQmlEngine *qml_engine, QJSEngine *js_engine)
    : InputParams(parent, qml_engine, js_engine)
    , roi_(new CVToolROI(this))
{
}

ImageProviderWrapper *CVInputParams::addImage(const QString &name, const QString &uuid, const QString &display_name,
                                              const QString &desc, const bool is_property, const bool visible)
{
    ImageProviderWrapper *provider = new ImageProviderWrapper(name, uuid, this, qml_engine_, js_engine_);

    bool ok = AbstractQuickToolParams::addParam(
        name, display_name, desc, paramtypes::QuickToolParamType::InputImageParamType, QVariant::fromValue(provider),
        QVariant(), true, is_property, true, visible);
    if (!ok)
    {
        delete provider;
        provider = nullptr;
    }

    return provider;
}

CVToolROI *CVInputParams::roi()
{
    return roi_;
}

CVOutputParams::CVOutputParams(QObject *parent, QQmlEngine *qml_engine, QJSEngine *js_engine)
    : OutputParams(parent, qml_engine, js_engine)
    , shapes_list_model_(new CVToolShapeListModel(this))
{
}

} // namespace quicktools::core
