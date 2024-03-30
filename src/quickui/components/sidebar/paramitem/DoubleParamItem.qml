import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

T_ParamItem {
    id: doubleItem
    displayText: _content.text
    QuickText {
        id: _content
        anchors.fill: parent
        text: paramValue === null || paramValue === undefined ? "" : paramValue
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
