#include "UITools.h"

#include <QColor>
#include <QUuid>

namespace quicktools::ui {

UITools::UITools(QObject *parent)
    : QObject{parent}
{
}

QColor UITools::withOpacity(const QColor &color, qreal opacity)
{
    int alpha = qRound(opacity * 255) & 0xff;
    return QColor::fromRgba((alpha << 24) | (color.rgba() & 0xffffff));
}

QString UITools::uuid()
{
    return QUuid::createUuid().toString().remove('-').remove('{').remove('}');
}

} // namespace quicktools::ui
