import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_ParamItem {
    id: intSpinBoxItem

    displayText: _content.value

    QuickSpinBox {
        id: _content

        anchors.fill: parent
        anchors.leftMargin: 2
        content.leftPadding: 5
        from: paramAdditional && paramAdditional.from ? paramAdditional.from : 0
        to: paramAdditional && paramAdditional.to ? paramAdditional.to : 0
        stepSize: paramAdditional && paramAdditional.stepSize ? paramAdditional.stepSize : 1
        value: paramValue ? paramValue : 0


        onValueModified: {
            valueChanged(_content.value);
        }
    }
}
