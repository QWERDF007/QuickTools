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
        editable: paramEditable
//        normalColor: QuickColor.Primary
//        hoverColor: Qt.lighter(normalColor, 1.1)
//        pressedColor: Qt.lighter(normalColor, 1.2)
        from: paramAdditional.from
        to: paramAdditional.to
        value: paramValue

        onValueModified: {
            intSpinBoxItem.valueChanged(_content.value)
        }
    }
}
