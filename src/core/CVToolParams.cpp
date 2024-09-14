#include "core/CVToolParams.h"

namespace quicktools::core {

CVInputParams::CVInputParams(QObject *parent)
    : InputParams(parent)
    , roi_(new CVToolROI(this))
{
}

CVToolROI *CVInputParams::roi()
{
    return roi_;
}

CVOutputParams::CVOutputParams(QObject *parent)
    : OutputParams(parent)
    , shapes_list_model_(new CVToolShapeListModel(this))
{
}

} // namespace quicktools::core
