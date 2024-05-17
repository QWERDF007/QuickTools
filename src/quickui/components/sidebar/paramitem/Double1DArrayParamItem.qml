import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_ParamItem {
    id: double1DArrayItem

    property int decimals: 4

    displayText: _content.text
    tooltipText: _content.text
    tooltipVisible: _content.truncated

    QuickText {
        id: _content

        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        text: paramDisplay
        verticalAlignment: Text.AlignVCenter
    }
}
