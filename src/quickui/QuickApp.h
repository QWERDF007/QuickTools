#pragma once

#include "Macros.h"

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QTranslator>

namespace quicktools::ui {

class QuickApp : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(QuickApp)
    QML_SINGLETON
public:
    /**
     * @brief 获取单例实例的指针
     * @return
     */
    static QuickApp *getInstance();

    /**
     * @brief 提供给 QML 创建一个单例实例的静态工厂函数
     * @param qmlEngine
     * @param jsEngine
     * @return
     */
    static QuickApp *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    Q_INVOKABLE void navigate(const QString &route, const QJsonObject &argument = {});
    Q_INVOKABLE void init(QObject *target, QLocale locale = QLocale::system());
    Q_INVOKABLE void exit(int exit_code = 0);
    Q_INVOKABLE void closeWindow(QObject *target);

    void addWindow(QQuickWindow *window);
    void removeWindow(QQuickWindow *window);

private:
    explicit QuickApp(QObject *parent = nullptr);
    ~QuickApp();

    Q_DISABLE_COPY_MOVE(QuickApp)

    static QuickApp *instance_;

    QMap<quint64, QQuickWindow *> windows_;
    QQmlEngine                   *engine_{nullptr};
    QTranslator                  *translator_{nullptr};
};

} // namespace quicktools::ui
