

#include "quickcolor.h"

namespace QuickTools::ui {

QuickColor *QuickColor::instance_ = nullptr;

/**
 * 注意不能使用返回静态变量的指针, 会报错 _CrtlsValidHeapPointer(block)
 */
QuickColor *QuickColor::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new QuickColor;
    }
    return instance_;
}

QuickColor::QuickColor(QObject *parent)
    : QObject(parent)
{
    //    Primary("#0066b4");
    Primary(QColor(0, 102, 180, 255));
    HighLight("#009688");
    FontPrimary(QColor(7, 7, 7, 255));
    WindowBackground(QColor(237, 237, 237, 255));
    WindowActiveBackground(QColor(243, 243, 243, 255));

    ItemNormal(QColor(0, 0, 0, 0));
    ItemHover(QColor(0, 0, 0, 255 * 0.03));
    ItemPress(QColor(0, 0, 0, 255 * 0.06));
    ItemCheck(QColor(0, 0, 0, 255 * 0.09));

    Grey120("#797775");
    Grey220("#11100f");
}

QuickColor::~QuickColor() {}

} // namespace QuickTools::ui
