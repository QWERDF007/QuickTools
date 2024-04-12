

#include "QuickColor.h"

namespace quicktools::ui {

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
    //    HighLight("#009688");
    FontPrimary(QColor(7, 7, 7, 255));
    WindowBackground(QColor(237, 237, 237, 255));
    WindowActiveBackground(QColor(243, 243, 243, 255));

    ItemNormal(QColor(0, 0, 0, 0)); // 透明
    ItemHover(QColor(0, 0, 0, 7));  // 255 * 0.03
    ItemPress(QColor(0, 0, 0, 15)); // 255 * 0.06
    ItemCheck(QColor(0, 0, 0, 22)); // 255 * 0.09

    Divider(QColor(210, 210, 210, 255));

    Transparent(QColor(0, 0, 0, 0));
    Black(QColor(0, 0, 0));
    White(QColor(255, 255, 255));
    Grey50(QColor(210, 208, 206));
    Grey80(QColor(179, 176, 173));
    Grey120(QColor(121, 119, 117));
    Grey170(QColor(41, 40, 39));
    Grey220(QColor(17, 16, 15));
}

QuickColor::~QuickColor() {}

} // namespace quicktools::ui
