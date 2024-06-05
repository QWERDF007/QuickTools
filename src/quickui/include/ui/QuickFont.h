#pragma once

#include "Macros.h"
#include "common/Singleton.h"

#include <QFont>
#include <QObject>
#include <QtQml>

namespace quicktools::ui {

class QuickFont : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(QuickFont)

    Q_PROPERTY_AUTO(QFont, Caption);
    Q_PROPERTY_AUTO(QFont, Body);
    Q_PROPERTY_AUTO(QFont, BodyStrong);
    Q_PROPERTY_AUTO(QFont, Subtitle);
    Q_PROPERTY_AUTO(QFont, Title);
    Q_PROPERTY_AUTO(QFont, TitleLarge);
    Q_PROPERTY_AUTO(QFont, Display);

public:
    QT_QML_SINGLETON(QuickFont)

private:
    explicit QuickFont(QObject *parent = nullptr);

    ~QuickFont() {}

    /**
     * 禁用拷贝和移动构造和运算符
     */
    Q_DISABLE_COPY_MOVE(QuickFont)
};

} // namespace quicktools::ui
