import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

T_ParamItem {
    id: statusItem
    displayText: _content.text
    QuickText {
        id: _content
        anchors.fill: parent
        text: {
            if (paramValue === null || paramValue === undefined) {
                return ""
            }
            return "[" + paramValue[0] + ", " + paramValue[1] + "]"
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
