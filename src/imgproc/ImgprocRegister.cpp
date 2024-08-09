#include "imgproc/ImgprocRegister.h"

#include "core/QuickToolManager.h"
#include "core/QuickToolType.h"
#include "imgproc/ImageHistogram.h"

namespace quicktools::imgproc {

void registerTools()
{
    core::QuickToolManager::getInstance()->registerGroupAndTask(core::quicktoolgrouptype::ImageProcessGroup,
                                                                core::quicktooltasktype::EmptyTask);
    registerImageHistogram();
}

} // namespace quicktools::imgproc
