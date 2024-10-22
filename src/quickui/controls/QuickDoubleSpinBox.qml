import QtQuick
import QuickTools.ui

QuickSpinBox {
    id: control
    property int decimals: 2
    readonly property int decimalFactor: Math.pow(10, decimals)
    property real realFrom: 0
    property real realTo: 0
    property real realValue: 0
    from: decimalToInt(realFrom)
    to: decimalToInt(realTo)
    // value: realValue * decimalFactor
    validator: DoubleValidator {
        bottom: Math.min(control.realFrom, control.realTo)
        top: Math.max(control.realFrom, control.realTo)
        decimals: control.decimals
        notation: DoubleValidator.StandardNotation
    }

    onValueChanged: {
        realValue = value / control.decimalFactor
    }
    
    textFromValue: function(value, locale) {
        return Number(value / control.decimalFactor).toLocaleString(locale, 'f', control.decimals)
    }

    valueFromText: function(text, locale) {
        return Math.round(Number.fromLocaleString(locale, text) * control.decimalFactor)
    }

    function decimalToInt(decimal) {
        return decimal * decimalFactor
    }

    Component.onCompleted: {
        value = realValue * decimalFactor
    }
}
