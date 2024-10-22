#include "samples/SamplesRegister.h"

#include "samples/PyTest.h"

namespace quicktools::samples {

void registerTools()
{
    core::QuickToolManager::getInstance()->registerGroupAndTask(core::quicktoolgrouptype::SamplesGroup,
                                                                core::quicktooltasktype::EmptyTask);
    registerPyTest();
}

} // namespace quicktools::samples
