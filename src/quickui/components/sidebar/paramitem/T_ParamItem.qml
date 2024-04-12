import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

import QuickTools.ui
import QuickTools.core


Item {
    id: paramItem
    anchors.fill: parent
    clip: true

    property int paramIndex
    property int paramType
    property var paramValue
    property var paramDisplay
    property var paramRange
    property string paramName: ""
    property alias displayText: textEdit.text
    property var mouseObj: mouseArea
    default property alias content: container.data

    property bool tooltipVisible: false
    property alias tooltipText: tooltip.text
    signal clicked
    signal valueChanged(var value)

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        hoverEnabled: true
        onClicked: {
            mouseMenu.open()
            paramItem.clicked()
            console.log("Click")
        }
    }
    QuickToolTip {
        id: tooltip
        visible: mouseArea.containsMouse && tooltipVisible
        delay: 500
    }

    TextEdit { // 用来当作剪贴板
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
                textEdit.selectAll()
                textEdit.copy()
            }
        }
    }
}
