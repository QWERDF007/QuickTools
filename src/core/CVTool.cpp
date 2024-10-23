#include "core/CVTool.h"

namespace quicktools::core {

AbstractCVTool::AbstractCVTool(QObject *parent, QQmlEngine *qml_engine, QJSEngine *js_engine)
    : AbstractTool(parent, qml_engine, js_engine)
{
}

AbstractCVTool::~AbstractCVTool() {}

} // namespace quicktools::core
