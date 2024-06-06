#include "imgproc/ImgprocRegister.h"

#include "imagehistogram/ImageHistogram.h"

namespace quicktools::imgproc {

void registerTools()
{
    core::QuickToolFactor::getInstance()->registerGroupAndTask(core::quicktoolgrouptype::ImageProcessGroup,
                                                               core::quicktooltasktype::EmptyTask);
    registerImageHistogram();
}

} // namespace quicktools::imgproc
