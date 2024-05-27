#pragma once
#include <QtQml>

namespace quicktools::core {

namespace quicktoolgrouptype {
Q_NAMESPACE

enum QuickToolGroupType
{
    EmptyGroup = 0,
    ImageProcessGroup, //!< 图像处理分组
    DeepLearningGroup, //!< 深度学习分组
};
Q_ENUM_NS(QuickToolGroupType) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中
QML_NAMED_ELEMENT(QuickToolGroupType)
} // namespace quicktoolgrouptype

namespace quicktooltasktype {
Q_NAMESPACE

enum QuickToolTaskType
{
    EmptyTask = 0,
};
Q_ENUM_NS(QuickToolTaskType)
QML_NAMED_ELEMENT(QuickToolTaskType)
} // namespace quicktooltasktype

namespace quicktooltype {
Q_NAMESPACE

enum QuickToolType
{
    EmptyTool          = 0,
    ImageHistogramTool = 1, //!< 图像直方图
};
Q_ENUM_NS(QuickToolType)
QML_NAMED_ELEMENT(QuickToolType)
} // namespace quicktooltype

} // namespace quicktools::core
