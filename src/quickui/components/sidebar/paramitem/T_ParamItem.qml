import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import QuickTools.ui
import QuickTools.core

Item {
    id: paramItem

    default property alias content: container.data
    property alias displayText: textEdit.text
    property var mouseObj: mouseArea
    property var paramDisplay
    property string paramName: ""
    property var paramRange
    property int paramType
    property var paramValue
    property alias tooltipText: tooltip.text
    property bool tooltipVisible: false

    signal clicked
    signal valueChanged(var value)

    anchors.fill: parent
    clip: true

    MouseArea {
        id: mouseArea

        acceptedButtons: Qt.RightButton
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            mouseMenu.open();
            paramItem.clicked();
        }
    }
    QuickToolTip {
        id: tooltip

        delay: 500
        visible: mouseArea.containsMouse && tooltipVisible
    }
    TextEdit {
        // 用来当作剪贴板
        id: textEdit

        visible: false
    }
    Item {
        id: container

        anchors.fill: parent
    }
    Menu {
        id: mouseMenu

        MenuItem {
            text: "复制"

            // shortcut: StandardKey.Copy
            onTriggered: {
                textEdit.selectAll();
                textEdit.copy();
            }
        }
    }
}
