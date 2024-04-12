import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

T_ParamItem {
    id: comboBoxItem
    displayText: _content.displayText
    tooltipVisible: _content.content.contentWidth > _content.content.width
    tooltipText: _content.displayText
    QuickComboBox {
        id: _content
        anchors.fill: parent
        anchors.leftMargin: 2
        // leftPadding: 0
        content.leftPadding: 5
        // bg.color: "transparent"
        bg.border.width: 0
        model: paramRange
        onActivated: function(index) {
            valueChanged(_content.displayText)
        }
    }
}
