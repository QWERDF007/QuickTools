import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Flickable {
    clip: true
    contentHeight: text.height
    contentWidth: parent.width
    ScrollBar.vertical: QuickScrollBar{}
    boundsBehavior: Flickable.StopAtBounds
    property url source
    QuickText {
        id: text
        width: parent.width
        wrapMode: Text.WrapAnywhere
        textFormat: TextEdit.MarkdownText
    }
}
