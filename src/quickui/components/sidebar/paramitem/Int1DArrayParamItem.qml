import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

T_ParamItem {
    id: int1DArrayItem
    displayText: _content.text
    tooltipVisible: _content.truncated
    tooltipText: _content.text
    QuickText {
        id: _content
        anchors.fill: parent
        text: {
            if (paramValue === null || paramValue === undefined) {
                return ""
            }
            var array_last_index = paramValue.length - 1
            var array_text = "["
            for (var i in paramValue) {
                if (i < array_last_index) {
                    array_text += paramValue[i] + ", "
                } else {
                    array_text += paramValue[i]
                }
            }
            array_text += "]"
            return array_text
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
