#include "QuickTools.h"
#include <QColor>

namespace quicktools::ui {

QuickUITools::QuickUITools(QObject *parent) : QObject{parent}
{

}


QColor QuickUITools::withOpacity(const QColor & color, qreal opacity)
{
    int alpha = qRound(opacity * 255) & 0xff;
    return QColor::fromRgba((alpha << 24) | (color.rgba() & 0xffffff));
}

} // namespace quicktools::ui
