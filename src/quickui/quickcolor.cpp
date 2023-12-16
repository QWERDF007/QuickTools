

#include "quickcolor.h"

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
    Primary("#0066b4");
    HighLight("#009688");
}

QuickColor::~QuickColor()
{
    qDebug() << __FUNCTION__ << "deconstructor";
}
