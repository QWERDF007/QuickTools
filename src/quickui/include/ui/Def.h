#pragma once

#include <QObject>
#include <QtQml>

namespace quicktools::ui { namespace button {
Q_NAMESPACE

enum ButtonFlag
{
    NeutralButton  = 0x0001,
    NegativeButton = 0x0002,
    PositiveButton = 0x0004
};
Q_ENUM_NS(ButtonFlag)
QML_NAMED_ELEMENT(QuickDialogButtonFlag)
}} // namespace quicktools::ui::button
