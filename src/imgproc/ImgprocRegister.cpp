#include "imgproc/ImgprocRegister.h"

#include "imgproc/ImageHistogram.h"

namespace quicktools::imgproc {

void registerTools()
{
    core::QuickToolManager::getInstance()->registerGroupAndTask(core::quicktoolgrouptype::ImageProcessGroup,
                                                                core::quicktooltasktype::EmptyTask);
    registerImageHistogram();
}

} // namespace quicktools::imgproc
