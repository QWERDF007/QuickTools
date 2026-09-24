import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

T_ParamItem {
    id: doubleSpinBoxItem

    displayText: _content.value

    QuiSpinEditor {
        id: _content
        anchors.fill: parent
        enabled: paramEditable
        decimals: paramAdditional && paramAdditional.decimals !== undefined ? paramAdditional.decimals : 2
        minValue: paramAdditional && paramAdditional.from !== undefined ? paramAdditional.from : 0.0
        maxValue: paramAdditional && paramAdditional.to !== undefined ? paramAdditional.to : 10000.0
        step: paramAdditional && paramAdditional.step !== undefined ? paramAdditional.step : 0.1
        value: paramValue !== undefined && paramValue !== null ? Number(paramValue) : 0.0

        onEditingFinished: {
            doubleSpinBoxItem.valueChanged(_content.value)
        }
    }
}
