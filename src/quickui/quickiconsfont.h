#pragma once

#include <QtQml/qqml.h>

#include <QObject>

namespace QuickTools {

// Windows Segoe Fluent 图标字体
// https://learn.microsoft.com/zh-cn/windows/apps/design/style/segoe-fluent-icons-font
namespace FontIcon {
Q_NAMESPACE
enum class FontIconType
{
    ChevronDown       = 0xe70d, //!< 人字形向下
    ChevronUp         = 0xe70e, //!< 人字形向上
    CheckboxComposite = 0xe73a, //!< 复选框, 勾选
    QRCode            = 0xed14, //!< 二维码
};
Q_ENUM_NS(FontIconType) // 向元对象系统注册枚举类型，必须在 Q_NAMESPACE 宏声明的命名空间中
QML_NAMED_ELEMENT(QuickFontIcon) // 声明封闭类型或命名空间在 QML 中可用，以 name 进行访问
} // namespace FontIcon

} // namespace QuickTools
