#include "deeplearning/DeepLearningRegister.h"

#include "core/QuickToolManager.h"
#include "core/QuickToolType.h"
#include "deeplearning/detection/Yolov8.h"

namespace quicktools::dl {

void registerTools()
{
    core::QuickToolManager::getInstance()->registerGroupAndTask(core::quicktoolgrouptype::DeepLearningGroup,
                                                                core::quicktooltasktype::DetectionTask);
    detection::registerYolov8Detection();
}

} // namespace quicktools::dl
