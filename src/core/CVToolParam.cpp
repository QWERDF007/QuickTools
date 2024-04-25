#include "CVToolParam.h"

namespace quicktools::core {

CVInputParams::CVInputParams(QObject *parent)
    : AbstractInputParams(parent)
    , roi_(new CVToolROI(this))
{
}

CVToolROI *CVInputParams::roi()
{
    return roi_;
}

CVOutputParams::CVOutputParams(QObject *parent)
    : AbstractOutputParams(parent)
{
}

} // namespace quicktools::core
