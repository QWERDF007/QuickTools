import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

T_ParamItem {
    id: intSpinBoxItem

    displayText: Math.round(_content.value)

    QuiSpinEditor {
        id: _content
        anchors.fill: parent
        enabled: paramEditable
        decimals: 0
        minValue: paramAdditional && paramAdditional.from !== undefined ? paramAdditional.from : 0
        maxValue: paramAdditional && paramAdditional.to !== undefined ? paramAdditional.to : 10000
        step: paramAdditional && paramAdditional.step !== undefined ? paramAdditional.step : 1
        value: paramValue !== undefined && paramValue !== null ? Number(paramValue) : 0

        onEditingFinished: {
            intSpinBoxItem.valueChanged(Math.round(_content.value))
        }
    }
}
