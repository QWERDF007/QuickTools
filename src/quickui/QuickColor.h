#pragma once

#include "Macros.h"

#include <QColor>
#include <QObject>
#include <QtQml>

namespace quicktools::ui {

class QuickColor : public QObject
{
    Q_OBJECT

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

    /**
     * 在 QML 中通过 QuickColor 可访问本单例
     */
    QML_NAMED_ELEMENT(QuickColor)

    /**
     * 在 QML 声明本类为单例
     */
    QML_SINGLETON

public:
    /**
     * @brief 获取单例实例的指针
     * @return
     */
    static QuickColor *getInstance();

    /**
     * @brief 提供给 QML 创建一个单例实例的静态工厂函数
     * @param qmlEngine
     * @param jsEngine
     * @return
     */
    static QuickColor *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)
        return getInstance();
    }

private:
    explicit QuickColor(QObject *parent = nullptr);
    ~QuickColor();

    /**
     * @brief 单例实例指针
     */
    static QuickColor *instance_;

    /**
     * 禁用拷贝和移动构造和运算符
     */
    Q_DISABLE_COPY_MOVE(QuickColor)
};

} // namespace quicktools::ui
