import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic

import QuickTools.ui

Button {
    property bool disabled: false
    property string contentDescription: ""
    property color normalColor: Qt.rgba(254/255,254/255,254/255,1)
    property color hoverColor:  Qt.rgba(246/255,246/255,246/255,1)
    property color disableColor: Qt.rgba(244/255,244/255,244/255,1)
    property color textColor: {
        if(!enabled){
            return Qt.rgba(160/255,160/255,160/255,1)
        }
        if(pressed){
            return Qt.rgba(96/255,96/255,96/255,1)
        }
        return Qt.rgba(0,0,0,1)
    }
    Accessible.role: Accessible.Button
    Accessible.name: control.text
    Accessible.description: contentDescription
    Accessible.onPressAction: control.clicked()
    id: control
    enabled: !disabled
    verticalPadding: 0
    horizontalPadding:12
    font: QuickFont.Body
    focusPolicy:Qt.TabFocus
    background: Rectangle{
        implicitWidth: 28
        implicitHeight: 28
        border.color: "#DFDFDF"
        border.width: 1
        radius: 4
        color:{
            if(!enabled){
                return disableColor
            }
            return hovered ? hoverColor :normalColor
        }
        QuickFocusRectangle {
            visible: control.activeFocus
            radius:4
        }
    }
    contentItem: QuickText {
        text: control.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font: control.font
        color: control.textColor
    }
}
