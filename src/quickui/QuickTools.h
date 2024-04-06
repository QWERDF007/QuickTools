#include "Singleton.h"

#include <QObject>
#include <QtQml>

namespace quicktools::ui {

class QuickUITools : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(QuickUITools)
    QML_SINGLETON
private:
    explicit QuickUITools(QObject *parent = nullptr);

public:
    SINGLETON(QuickUITools)

    // static QuickUITools *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    // {
    //     Q_UNUSED(qmlEngine)
    //     Q_UNUSED(jsEngine)
    //     return getInstance();
    // }

    Q_INVOKABLE QColor withOpacity(const QColor & color, qreal opacity);
};
} // namespace quicktools::ui
