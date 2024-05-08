#pragma once

#include "Singleton.h"

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QTranslator>

namespace quicktools::ui {

class App : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(App)
    QML_SINGLETON
public:
    QT_QML_SINGLETON(App)

    Q_INVOKABLE void navigate(const QString &route, const QJsonObject &argument = {});
    Q_INVOKABLE void init(QObject *target, QLocale locale = QLocale::system());
    Q_INVOKABLE void exit(int exit_code = 0);
    Q_INVOKABLE void closeWindow(QObject *target);

    void addWindow(QQuickWindow *window);
    void removeWindow(QQuickWindow *window);

private:
    explicit App(QObject *parent = nullptr);
    ~App();

    Q_DISABLE_COPY_MOVE(App)

    //    static App *instance_;

    QMap<quint64, QQuickWindow *> windows_;
    QQmlEngine                   *engine_{nullptr};
    QTranslator                  *translator_{nullptr};
};

} // namespace quicktools::ui
