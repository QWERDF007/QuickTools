#pragma once

#include "Macros.h"
#include "common/Singleton.h"

#include <QColor>
#include <QObject>
#include <QtQml>

namespace quicktools::ui {

class QuickColor : public QObject
{
    Q_OBJECT
    // 声明在 QML 中本类是单例
    QML_SINGLETON
    // 声明在 QML 中可以通过 QuickColor 进行访问
    QML_NAMED_ELEMENT(QuickColor)

    Q_PROPERTY_AUTO(QColor, Primary)
    //    Q_PROPERTY_AUTO(QColor, HighLight)
    Q_PROPERTY_AUTO(QColor, FontPrimary)
    Q_PROPERTY_AUTO(QColor, FontCaption)
    Q_PROPERTY_AUTO(QColor, WindowBackground)
    Q_PROPERTY_AUTO(QColor, WindowActiveBackground)

    Q_PROPERTY_AUTO(QColor, ItemNormal)
    Q_PROPERTY_AUTO(QColor, ItemHover)
    Q_PROPERTY_AUTO(QColor, ItemPress)
    Q_PROPERTY_AUTO(QColor, ItemCheck)
    Q_PROPERTY_AUTO(QColor, ItemDisabled)

    Q_PROPERTY_AUTO(QColor, Divider)

    Q_PROPERTY_AUTO(QColor, Transparent)
    Q_PROPERTY_AUTO(QColor, Black)
    Q_PROPERTY_AUTO(QColor, White)

    Q_PROPERTY_AUTO(QColor, Grey10)
    Q_PROPERTY_AUTO(QColor, Grey20)
    Q_PROPERTY_AUTO(QColor, Grey30)
    Q_PROPERTY_AUTO(QColor, Grey40)
    Q_PROPERTY_AUTO(QColor, Grey50)
    Q_PROPERTY_AUTO(QColor, Grey60)
    Q_PROPERTY_AUTO(QColor, Grey70)
    Q_PROPERTY_AUTO(QColor, Grey80)
    Q_PROPERTY_AUTO(QColor, Grey90)
    Q_PROPERTY_AUTO(QColor, Grey100)
    Q_PROPERTY_AUTO(QColor, Grey110)
    Q_PROPERTY_AUTO(QColor, Grey120)
    Q_PROPERTY_AUTO(QColor, Grey130)
    Q_PROPERTY_AUTO(QColor, Grey140)
    Q_PROPERTY_AUTO(QColor, Grey150)
    Q_PROPERTY_AUTO(QColor, Grey160)
    Q_PROPERTY_AUTO(QColor, Grey170)
    Q_PROPERTY_AUTO(QColor, Grey180)
    Q_PROPERTY_AUTO(QColor, Grey190)
    Q_PROPERTY_AUTO(QColor, Grey200)
    Q_PROPERTY_AUTO(QColor, Grey210)
    Q_PROPERTY_AUTO(QColor, Grey220)

public:
    // 声明并定义 QML 单例所需的成员变量/函数
    QT_QML_SINGLETON(QuickColor)

private:
    explicit QuickColor(QObject *parent = nullptr);
    ~QuickColor();

    /**
     * 禁用拷贝和移动构造和运算符
     */
    Q_DISABLE_COPY_MOVE(QuickColor)
};

} // namespace quicktools::ui
