import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Item {
    id: paramHeader
    width: 200
    height: 24
    property string paramDisplayName: ""
    property string paramTypeName: ""
    QuickText {
        anchors.fill: parent
        text: paramDisplayName
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    ToolTip {
        visible: paramTypeName !== "" && mouseArea.containsMouse
        delay: 200
        text: "参数类型: " + paramTypeName
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}
