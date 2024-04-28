import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Item {
    property alias text: text.text
    Flickable {
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.leftMargin: 5
        clip: true
        contentHeight: text.height
        contentWidth: width
        ScrollBar.vertical: QuickScrollBar{}
        boundsBehavior: Flickable.StopAtBounds
        QuickText {
            id: text
            width: parent.width
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 5
            wrapMode: Text.WrapAnywhere
            textFormat: TextEdit.MarkdownText
        }
    }
}
