#include "ui/App.h"

#include <QQuickItem>

namespace quicktools::ui {

void App::navigate(const QString &route, const QJsonObject &argument)
{
    QQmlComponent component(engine_, route);
    if (component.isError())
    {
        qCritical() << __FUNCTION__ << __LINE__ << component.errors();
        return;
    }
    QVariantMap   properties;
    QQuickWindow *window{nullptr};
    window = qobject_cast<QQuickWindow *>(component.createWithInitialProperties(properties));
    if (window)
    {
        if (argument.contains("icon"))
            window->setIcon(QIcon(":" + argument.value("icon").toString()));
        windows_.insert(window->winId(), window);
        // window->show();
    }
}

void App::init(QObject *target, QLocale locale)
{
    Q_UNUSED(locale)
    engine_ = qmlEngine(target);
}

void App::exit(int exit_code)
{
    for (const auto &[id, window] : windows_.toStdMap())
    {
        window->close();
        removeWindow(window);
    }
    qApp->exit(exit_code);
}

void App::closeWindow(QObject *target)
{
    QQuickWindow *window{nullptr};
    window = qobject_cast<QQuickWindow *>(target);
    removeWindow(window);
}

void App::addWindow(QQuickWindow *window)
{
    windows_.insert(window->winId(), window);
}

void App::removeWindow(QQuickWindow *window)
{
    if (window)
    {
        windows_.remove(window->winId());
        window->deleteLater();
        window = nullptr;
    }
}

App::App(QObject *parent)
    : QObject(parent)
{
}

App::~App() {}

} // namespace quicktools::ui
