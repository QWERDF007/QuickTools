#include "QuickApp.h"

#include <QQuickItem>

namespace quicktools::ui {

QuickApp *QuickApp::instance_ = nullptr;

QuickApp *QuickApp::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new QuickApp;
    }
    return instance_;
}

QuickApp *QuickApp::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)
    return getInstance();
}

void QuickApp::navigate(const QString &route, const QJsonObject &argument)
{
    Q_UNUSED(argument)
    QQmlComponent component(engine_, route);
    if (component.isError())
    {
        qCritical() << __FUNCTION__ << component.errors();
        return;
    }
    QVariantMap   properties;
    QQuickWindow *window{nullptr};
    window = qobject_cast<QQuickWindow *>(component.createWithInitialProperties(properties));
    if (window)
    {
        windows_.insert(window->winId(), window);
        // window->show();
    }
}

void QuickApp::init(QObject *target, QLocale locale)
{
    Q_UNUSED(locale)
    engine_ = qmlEngine(target);
}

void QuickApp::exit(int ret_code)
{
    for (const auto &[id, window] : windows_.toStdMap())
    {
        window->close();
        removeWindow(window);
    }
    qApp->exit(ret_code);
}

void QuickApp::addWindow(QQuickWindow *window)
{
    windows_.insert(window->winId(), window);
}

void QuickApp::removeWindow(QQuickWindow *window)
{
    if (window)
    {
        windows_.remove(window->winId());
        window->deleteLater();
        window = nullptr;
    }
}

QuickApp::QuickApp(QObject *parent)
    : QObject(parent)
{
}

QuickApp::~QuickApp() {}

} // namespace quicktools::ui
