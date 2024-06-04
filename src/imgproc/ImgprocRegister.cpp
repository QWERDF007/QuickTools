#include "imgprocregister.h"

#include "imagehistogram/ImageHistogram.h"
#include "pytest/PyTest.h"

namespace quicktools::imgproc {

void registerTools()
{
    core::QuickToolFactor::getInstance()->registerGroupAndTask(core::quicktoolgrouptype::ImageProcessGroup,
                                                               core::quicktooltasktype::EmptyTask);
    registerImageHistogram();
    registerPyTest();
}

} // namespace quicktools::imgproc
