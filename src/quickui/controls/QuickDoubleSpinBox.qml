import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

import QuickTools.ui

T.SpinBox {
    id: control
    property int decimals: 2
    readonly property int decimalFactor: Math.pow(10, decimals)
    property real realFrom: 0
    property real realTo: 0
    property real realValue: 0
    from: decimalToInt(realFrom)
    to: decimalToInt(realTo)
    value: realValue * decimalFactor
    property alias content: _content
    property bool disabled: false
    property color normalColor: Qt.rgba(232/255,232/255,232/255,1)
    property color hoverColor: Qt.rgba(224/255,224/255,224/255,1)
    property color pressedColor: Qt.rgba(216/255,216/255,216/255,1)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             up.implicitIndicatorHeight, down.implicitIndicatorHeight)
    leftPadding: padding + (control.mirrored ? (up.indicator ? up.indicator.width : 0) : (down.indicator ? down.indicator.width : 0))
    rightPadding: padding + (control.mirrored ? (down.indicator ? down.indicator.width : 0) : (up.indicator ? up.indicator.width : 0))
    enabled: !disabled
    validator: DoubleValidator {
        bottom: Math.min(control.realFrom, control.realTo)
        top: Math.max(control.realFrom, control.realTo)
        decimals: control.decimals
        notation: DoubleValidator.StandardNotation
    }

    onValueChanged: {
        realValue = value / control.decimalFactor
    }

    function decimalToInt(decimal) {
        return decimal * decimalFactor
    }

    textFromValue: function(value, locale) {
        return Number(value / control.decimalFactor).toLocaleString(locale, 'f', control.decimals)
    }
    valueFromText: function(text, locale) {
        return Math.round(Number.fromLocaleString(locale, text) * control.decimalFactor)
    }


    font: QuickFont.Body

    contentItem: TextInput {
        id: _content
        property color normalColor: Qt.rgba(27/255,27/255,27/255,1)
        property color disableColor: Qt.rgba(160/255,160/255,160/255,1)
        property color placeholderNormalColor: Qt.rgba(96/255,96/255,96/255,1)
        property color placeholderFocusColor: Qt.rgba(141/255,141/255,141/255,1)
        property color placeholderDisableColor: Qt.rgba(160/255,160/255,160/255,1)
        z: 2
        text: control.displayText
        clip: width < implicitWidth
        padding: 6
        font: control.font
        color: {
            if(!enabled){
                return disableColor
            }
            return normalColor
        }
        selectionColor: UITools.withOpacity(QuickColor.Primary,0.5)
        selectedTextColor: color
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: control.inputMethodHints
        Rectangle{
            width: parent.width
            height: contentItem.activeFocus ? 2 : 1
            anchors.bottom: parent.bottom
            visible: contentItem.enabled
            color: {
                if(contentItem.activeFocus){
                    return QuickColor.Primary
                }
                return Qt.rgba(183/255,183/255,183/255,1)
            }
            Behavior on height{
                enabled: true
                NumberAnimation{
                    duration: 83
                    easing.type: Easing.OutCubic
                }
            }
        }
    }

    up.indicator: QuickClip {
        x: control.mirrored ? 0 : control.width - width
        height: control.height
        implicitWidth: 32
        implicitHeight: 32
        radius: [0,4,4,0]
        Rectangle{
            anchors.fill: parent
            color: {
                if(control.up.pressed){
                    return enabled ? control.pressedColor : Qt.rgba(216/255,216/255,216/255,1)
                }
                if(control.up.hovered){
                    return enabled ? control.hoverColor : Qt.rgba(224/255,224/255,224/255,1)
                }
                return enabled ? control.normalColor : Qt.rgba(232/255,232/255,232/255,1)
            }
        }
        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: parent.width / 3
            height: 2
            color: enabled ? Qt.rgba(0,0,0,1) : QuickColor.Grey90
        }
        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 2
            height: parent.width / 3
            color: enabled ? Qt.rgba(0,0,0,1) : QuickColor.Grey90
        }
    }


    down.indicator: QuickClip {
        x: control.mirrored ? parent.width - width : 0
        height: control.height
        implicitWidth: 32
        implicitHeight: 32
        radius: [4,0,0,4]
        Rectangle{
            anchors.fill: parent
            color: {
                if(control.down.pressed){
                    return enabled ? control.pressedColor : Qt.rgba(216/255,216/255,216/255,1)
                }
                if(control.down.hovered){
                    return enabled ? control.hoverColor : Qt.rgba(224/255,224/255,224/255,1)
                }
                return enabled ? control.normalColor : Qt.rgba(232/255,232/255,232/255,1)
            }
        }
        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: parent.width / 3
            height: 2
            color: enabled ? Qt.rgba(0,0,0,1) : QuickColor.Grey90
        }
    }

    background: Rectangle {
        implicitWidth: 136
        radius: 4
        border.width: 1
        border.color: {
            if(contentItem.disabled){
                return Qt.rgba(237/255,237/255,237/255,1)
            }
            return Qt.rgba(240/255,240/255,240/255,1)
        }
        color: {
            if(contentItem.disabled){
                return Qt.rgba(252/255,252/255,252/255,1)
            }
            if(contentItem.activeFocus){
                return Qt.rgba(1,1,1,1)
            }
            if(contentItem.hovered){
                return Qt.rgba(251/255,251/255,251/255,1)
            }
            return Qt.rgba(1,1,1,1)
        }
    }
}
