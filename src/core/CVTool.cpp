#include "core/CVTool.h"

namespace quicktools::core {

AbstractCVTool::AbstractCVTool(QObject *parent)
    : AbstractTool(parent)
{
}

AbstractCVTool::~AbstractCVTool() {}

} // namespace quicktools::core
