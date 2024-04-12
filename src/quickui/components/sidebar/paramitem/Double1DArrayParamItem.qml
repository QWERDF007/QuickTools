import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

T_ParamItem {
    id: double1DArrayItem
    displayText: _content.text
    property int decimals: 4
    tooltipVisible: _content.truncated
    tooltipText: _content.text

    QuickText {
        id: _content
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        text: paramDisplay
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
