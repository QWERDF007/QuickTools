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
    background: QuickControlBackground{
        implicitWidth: 30
        implicitHeight: 30
        radius: 4
        bottomMargin: enabled ? 2 : 0
        border.width: enabled ? 1 : 0
        border.color: enabled ? Qt.darker(control.normalColor,1.2) : disableColor
        color:{
            if(!enabled){
                return disableColor
            }
            if(pressed){
                return pressedColor
            }
            return hovered ? hoverColor :normalColor
        }
        QuickFocusRectangle{
            visible: control.visualFocus
            radius:4
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
