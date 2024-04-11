import QtQuick
import QtQuick.Controls

import QuickTools.ui

Text {
    property color textColor: QuickColor.FontPrimary
    id:text
    color: textColor
    renderType: Text.NativeRendering
    font: QuickFont.Body
}
