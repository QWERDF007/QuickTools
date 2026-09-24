import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

T_ParamItem {
    id: double2DArrayItem

    property int decimals: 4

    displayText: _content.text
    tooltipText: _content.text
    tooltipVisible: _content.truncated

    QuiText {
        id: _content

        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        text: paramDisplay !== undefined && paramDisplay !== null ? String(paramDisplay) : ""
        verticalAlignment: Text.AlignVCenter
        font: QuiFont.Caption
        color: QuiColor.FontPrimary
    }
}
