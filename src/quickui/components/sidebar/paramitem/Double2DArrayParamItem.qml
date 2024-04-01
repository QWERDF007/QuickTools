import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

T_ParamItem {
    id: double2DArrayItem
    displayText: _content.text
    property int decimals: 4
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
                var sub_array_last_index = paramValue[i].length - 1
                var sub_array_text = "["
                for (var j in paramValue[i]) {
                    if (j < sub_array_last_index) {
                        sub_array_text += paramValue[i][j].toFixed(decimals) + ", "
                    } else {
                        sub_array_text += paramValue[i][j].toFixed(decimals)
                    }
                }
                sub_array_text += "]"
                if (i < array_last_index) {
                    array_text += sub_array_text + ", "
                } else {
                    array_text += sub_array_text
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
