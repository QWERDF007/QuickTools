#include "core/CVToolParams.h"

namespace quicktools::core {

CVInputParams::CVInputParams(QObject *parent, QQmlEngine * qml_engine, QJSEngine* js_engine)
    : InputParams(parent, qml_engine, js_engine)
    , roi_(new CVToolROI(this))
{
}

CVToolROI *CVInputParams::roi()
{
    return roi_;
}

CVOutputParams::CVOutputParams(QObject *parent, QQmlEngine * qml_engine, QJSEngine* js_engine)
    : OutputParams(parent, qml_engine, js_engine)
    , shapes_list_model_(new CVToolShapeListModel(this))
{
}

} // namespace quicktools::core
