import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_ParamItem {
    id: doubleSpinBoxItem

    displayText: _content.value

    QuickDoubleSpinBox {
        id: _content
        anchors.fill: parent
        editable: paramEditable
//        normalColor: QuickColor.Primary
//        hoverColor: Qt.lighter(normalColor, 1.1)
//        pressedColor: Qt.lighter(normalColor, 1.2)
        decimals: paramAdditional.decimals
        realFrom: paramAdditional.from
        realTo: paramAdditional.to
        realValue: paramValue

        onValueModified: {
            doubleSpinBoxItem.valueChanged(_content.realValue)
        }
    }
}
