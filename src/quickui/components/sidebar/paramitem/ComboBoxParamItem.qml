import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_ParamItem {
    id: comboBoxItem

    displayText: _content.displayText
    tooltipText: _content.displayText
    tooltipVisible: _content.content.contentWidth > _content.content.width

    QuickComboBox {
        id: _content

        anchors.fill: parent
        anchors.leftMargin: 2
        // bg.color: "transparent"
        bg.border.width: 0
        // leftPadding: 0
        content.leftPadding: 5
        model: paramAdditional.model

        onActivated: function (index) {
            valueChanged(_content.displayText);
        }

        Component.onCompleted: {
            currentIndex = indexOfValue(paramDisplay)
        }
    }
}
