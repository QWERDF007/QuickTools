#include "common/Singleton.h"

#include <QObject>
#include <QtQml>

namespace quicktools::ui {

class UITools : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(UITools)
public:
    QT_QML_SINGLETON(UITools)

    Q_INVOKABLE QColor withOpacity(const QColor &color, qreal opacity);

    Q_INVOKABLE QString uuid();

private:
    explicit UITools(QObject *parent = nullptr);

    ~UITools() {}
};
} // namespace quicktools::ui
