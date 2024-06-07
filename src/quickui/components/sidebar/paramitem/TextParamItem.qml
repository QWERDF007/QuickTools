import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_ParamItem {
    id: textItem

    displayText: loader.item ? loader.item.text : ""
    tooltipText: loader.item ? loader.item.text : ""
    tooltipVisible: loader.item ? loader.item.contentWidth > loader.item.width : false

    Component {
        id: text_com
        QuickText {
            id: _content

            anchors.fill: parent

            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            text: paramDisplay
            verticalAlignment: Text.AlignVCenter
        }
    }

    Component {
        id: textinput_com
        TextInput {
            id: textinput

            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            text: paramDisplay
            verticalAlignment: Text.AlignVCenter

            onEditingFinished: {
                if (paramDisplay !== textinput.text) {
                    valueChanged(textinput.text)
                }
            }
        }
    }

    QuickLoader {
        id: loader
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.fill: parent
        sourceComponent: paramEditable ? textinput_com : text_com
    }
}
