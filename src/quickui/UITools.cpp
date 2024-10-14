#include "ui/UITools.h"

#include "common/Utils.h"

#include <QColor>
#include <QUuid>

namespace quicktools::ui {

UITools::UITools(QObject *parent)
    : QObject{parent}
{
}

QColor UITools::withOpacity(const QColor &color, qreal opacity) const
{
    int alpha = qRound(opacity * 255) & 0xff;
    return QColor::fromRgba((alpha << 24) | (color.rgba() & 0xffffff));
}

QString UITools::uuid() const
{
    return common::uuid();
}

QString UITools::getDirectory(const QString &path) const
{
    return common::getDirectory(path);
}

} // namespace quicktools::ui
