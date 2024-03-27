import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Item {
    id: paramHeader
    width: 200
    height: 24
    property string paramName: ""
    property string paramTypeName: ""
    QuickText {
        anchors.fill: parent
        text: paramHeader.paramName
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    ToolTip {
        visible: paramHeader.paramTypeName !== null && paramHeader.paramTypeName != undefined && paramHeader.paramTypeName !== "" && mouseArea.containsMouse
        delay: 200
        text: "参数类型: " + paramHeader.paramTypeName
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}
