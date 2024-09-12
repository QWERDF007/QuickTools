#pragma once

#include <QtQml>

namespace quicktools::core {

namespace quicktoolgrouptype {
Q_NAMESPACE

enum QuickToolGroupType
{
    EmptyGroup = 0,    //!< 空分组
    ImageProcessGroup, //!< 图像处理分组
    DeepLearningGroup, //!< 深度学习分组
    SamplesGroup,      //!< 示例分组
};
Q_ENUM_NS(QuickToolGroupType) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中
QML_NAMED_ELEMENT(QuickToolGroupType)
} // namespace quicktoolgrouptype

namespace quicktooltasktype {
Q_NAMESPACE

enum QuickToolTaskType
{
    EmptyTask = 0, //!< 空任务
    DetectionTask, //!< 检测任务
};
Q_ENUM_NS(QuickToolTaskType)
QML_NAMED_ELEMENT(QuickToolTaskType)
} // namespace quicktooltasktype

namespace quicktooltype {
Q_NAMESPACE

enum QuickToolType
{
    EmptyTool = 0,      //!< 空工具
    ImageHistogramTool, //!< 图像直方图
    Yolov8DetectionTool,
    PyTestTool,
};
Q_ENUM_NS(QuickToolType)
QML_NAMED_ELEMENT(QuickToolType)
} // namespace quicktooltype

namespace shapetype {
Q_NAMESPACE

enum ShapeType
{
    NoShape   = 0x00000000, //!< 没有任何形状
    Rectangle = 0x00000001, //!< 矩形
    Circle    = 0x00000002, //!< 圆形
    Polygon   = 0x00000004, //!< 多边形
    AllShapes = 0x07ffffff, //!< 所有形状
};
Q_ENUM_NS(ShapeType)
QML_NAMED_ELEMENT(ShapeType)
} // namespace shapetype

} // namespace quicktools::core
