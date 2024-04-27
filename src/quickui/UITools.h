#include "Singleton.h"

#include <QObject>
#include <QtQml>

namespace quicktools::ui {

class UITools : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(UITools)
    QML_SINGLETON
private:
    explicit UITools(QObject *parent = nullptr);

public:
    SINGLETON(UITools)

    // static UITools *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    // {
    //     Q_UNUSED(qmlEngine)
    //     Q_UNUSED(jsEngine)
    //     return getInstance();
    // }

    Q_INVOKABLE QColor withOpacity(const QColor &color, qreal opacity);
};
} // namespace quicktools::ui
