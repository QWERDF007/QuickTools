import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_ParamItem {
    id: intSpinBoxItem

    displayText: _content.value

    SpinBox {

    }

    QuickSpinBox {
        id: _content
        anchors.fill: parent
        editable: paramEditable
        from: paramAdditional.from
        to: paramAdditional.to
        stepSize: paramAdditional.stepSize
        value: paramValue

        onValueModified: {
            intSpinBoxItem.valueChanged(_content.value);
        }
    }
}
