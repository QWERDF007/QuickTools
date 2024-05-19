

#include "QuickColor.h"

namespace quicktools::ui {

QuickColor *QuickColor::instance_ = nullptr;

/**
 * @note: 不能使用返回静态局部变量的指针, 否则结束时会报错 _CrtlsValidHeapPointer(block),
 *        应该是 Qt 对指针进行了 delete, 然后结束时静态变量又自己 delete, 导致 double delete.
 * @note: 此实现不是Meyers' Singleton! 可能存在问题?
 *        参考: https://www.zhihu.com/question/56527586/answer/2344903391
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
    FontCaption(QColor(118, 118, 118, 255));
    WindowBackground(QColor(237, 237, 237, 255));
    WindowActiveBackground(QColor(243, 243, 243, 255));

    ItemNormal(QColor(0, 0, 0, 0)); // 透明
    ItemHover(QColor(0, 0, 0, 7));  // 255 * 0.03
    ItemPress(QColor(0, 0, 0, 15)); // 255 * 0.06
    ItemCheck(QColor(0, 0, 0, 22)); // 255 * 0.09
    ItemDisabled(QColor(225, 223, 221));

    Divider(QColor(210, 210, 210, 255));

    Transparent(QColor(0, 0, 0, 0));
    Black(QColor(0, 0, 0));
    White(QColor(255, 255, 255));

    Grey10(QColor(250, 249, 248));
    Grey20(QColor(243, 242, 241));
    Grey30(QColor(237, 235, 233));
    Grey40(QColor(225, 223, 221));
    Grey50(QColor(210, 208, 206));
    Grey60(QColor(200, 198, 196));
    Grey70(QColor(190, 185, 184));
    Grey80(QColor(179, 176, 173));
    Grey90(QColor(161, 159, 157));
    Grey100(QColor(151, 149, 146));
    Grey110(QColor(138, 136, 134));
    Grey120(QColor(121, 119, 117));
    Grey130(QColor(96, 94, 92));
    Grey140(QColor(72, 70, 68));
    Grey150(QColor(59, 58, 57));
    Grey160(QColor(50, 49, 48));
    Grey170(QColor(41, 40, 39));
    Grey180(QColor(37, 36, 35));
    Grey190(QColor(32, 31, 30));
    Grey200(QColor(27, 26, 25));
    Grey210(QColor(22, 21, 20));
    Grey220(QColor(17, 16, 15));
}

QuickColor::~QuickColor() {}

} // namespace quicktools::ui
