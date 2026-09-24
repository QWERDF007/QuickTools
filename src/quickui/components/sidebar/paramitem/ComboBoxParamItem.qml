import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

T_ParamItem {
    id: comboBoxItem

    displayText: _content.displayText
    tooltipText: _content.displayText
    tooltipVisible: _content.contentItem ? _content.contentItem.contentWidth > _content.contentItem.width : false

    QuiComboBox {
        id: _content

        anchors.fill: parent
        anchors.leftMargin: 2
        model: paramAdditional ? paramAdditional.model : []

        onActivated: function (index) {
            valueChanged(_content.displayText);
        }

        Component.onCompleted: {
            currentIndex = indexOfValue(paramDisplay)
        }
    }
}
