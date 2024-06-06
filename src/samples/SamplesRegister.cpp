#include "samples/SamplesRegister.h"

#include "PyTest/PyTest.h"

namespace quicktools::samples {

void registerTools()
{
    core::QuickToolFactor::getInstance()->registerGroupAndTask(core::quicktoolgrouptype::ImageProcessGroup,
                                                               core::quicktooltasktype::EmptyTask);
    registerPyTest();
}

} // namespace quicktools::samples
