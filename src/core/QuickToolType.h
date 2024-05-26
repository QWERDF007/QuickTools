#pragma once
#include <QtQml>

namespace quicktools::core {

namespace quicktooltypes {
Q_NAMESPACE

enum QuickToolGroupType
{
    EmptyGroup = 0,
    ImageProcess, //!< 图像处理分组
    DeepLearning, //!< 深度学习分组
};
Q_ENUM_NS(QuickToolGroupType) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中
QML_NAMED_ELEMENT(QuickToolGroupType) // 声明封闭类型或命名空间在 QML 中可用，以 name 进行访问

enum QuickToolTaskType
{
    EmptyTask = 0,
};
Q_ENUM_NS(QuickToolTaskType)
QML_NAMED_ELEMENT(QuickToolTaskType)

enum QuickToolType
{
    EmptyTool = 0,
    ImageHistogram = 1, //!< 图像直方图
};
Q_ENUM_NS(QuickToolType)
QML_NAMED_ELEMENT(QuickToolType)

} // namespace quicktools::core::quicktooltypes
} // namespace quicktools::core
