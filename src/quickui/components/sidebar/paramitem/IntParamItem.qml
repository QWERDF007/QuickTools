import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_ParamItem {
    id: intItem

    displayText: _content.text

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
