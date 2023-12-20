#pragma once

#include "macros.h"

#include <QFont>
#include <QObject>
#include <QtQml>

namespace QuickTools::ui {

class QuickFont : public QObject
{
    Q_OBJECT

    Q_PROPERTY_AUTO(QFont, Caption);
    Q_PROPERTY_AUTO(QFont, Body);
    Q_PROPERTY_AUTO(QFont, BodyStrong);
    Q_PROPERTY_AUTO(QFont, Subtitle);
    Q_PROPERTY_AUTO(QFont, Title);
    Q_PROPERTY_AUTO(QFont, TitleLarge);
    Q_PROPERTY_AUTO(QFont, Display);

    /**
     * 在 QML 中通过 QuickColor 可访问本单例
     */
    QML_NAMED_ELEMENT(QuickFont)

    /**
     * 在 QML 声明本类为单例
     */
    QML_SINGLETON

public:
    /**
     * @brief 获取单例实例的指针
     * @return
     */
    static QuickFont *getInstance();

    /**
     * @brief 提供给 QML 创建一个单例实例的静态工厂函数
     * @param qmlEngine
     * @param jsEngine
     * @return
     */
    static QuickFont *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        return getInstance();
    }

private:
    explicit QuickFont(QObject *parent = nullptr);
    ~QuickFont();

    /**
     * @brief 单例实例指针
     */
    static QuickFont *instance_;

    /**
     * 禁用拷贝和移动构造和运算符
     */
    Q_DISABLE_COPY_MOVE(QuickFont)
};

} // namespace QuickTools::ui
