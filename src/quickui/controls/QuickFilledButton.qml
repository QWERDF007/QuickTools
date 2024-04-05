import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic

import QuickTools.ui

Button {
    property bool disabled: false
    property string contentDescription: ""
    property color normalColor: QuickColor.Primary
    property color hoverColor: Qt.lighter(normalColor,1.1)
    property color disableColor: Qt.rgba(199/255,199/255,199/255,1)
    property color pressedColor: Qt.lighter(normalColor,1.2)
    property color textColor: Qt.rgba(1,1,1,1)
    Accessible.role: Accessible.Button
    Accessible.name: control.text
    Accessible.description: contentDescription
    Accessible.onPressAction: control.clicked()
    id: control
    enabled: !disabled
    focusPolicy:Qt.TabFocus
    font: QuickFont.Body
    verticalPadding: 0
    horizontalPadding:12
    background: Rectangle{
        implicitWidth: 28
        implicitHeight: 28
        radius: 4
        QuickFocusRectangle{
            visible: control.visualFocus
            radius:4
        }
        gradient: Gradient {
            GradientStop { position: 0.33; color: control.enabled ? control.normalColor : Qt.rgba(0,0,0,0) }
            GradientStop { position: 1.0; color: control.enabled ? Qt.darker(control.normalColor,1.3) : Qt.rgba(0,0,0,0) }
        }
        Rectangle{
            radius: parent.radius
            anchors{
                fill: parent
                topMargin: control.enabled ? 0 : 0
                leftMargin: control.enabled ? 1 : 0
                rightMargin: control.enabled ? 1 : 0
                bottomMargin: control.enabled ? 2 : 0
            }
            color:{
                if(!enabled){
                    return disableColor
                }
                if(pressed){
                    return pressedColor
                }
                return hovered ? hoverColor :normalColor
            }
        }
    }
    contentItem: QuickText {
        text: control.text
        font: control.font
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: control.textColor
    }
}
