import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

Item {
    property alias text: text.text

    Flickable {
        anchors.bottomMargin: 5
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.topMargin: 5
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        contentHeight: text.height
        contentWidth: width

        ScrollBar.vertical: QuickScrollBar {
        }

        QuickText {
            id: text

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 5
            textFormat: TextEdit.MarkdownText
            width: parent.width
            wrapMode: Text.WrapAnywhere
        }
    }
}
