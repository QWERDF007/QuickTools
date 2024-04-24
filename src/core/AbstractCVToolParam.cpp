#include "AbstractCVToolparam.h"

namespace quicktools::core {

AbstractCVInputParams::AbstractCVInputParams(QObject *parent)
    : AbstractInputParams(parent)
    , roi_(new CVToolROI(this))
{
}

CVToolROI *AbstractCVInputParams::roi()
{
    return roi_;
}

AbstractCVOutputParams::AbstractCVOutputParams(QObject *parent)
    : AbstractOutputParams(parent)
{
}

} // namespace quicktools::core
