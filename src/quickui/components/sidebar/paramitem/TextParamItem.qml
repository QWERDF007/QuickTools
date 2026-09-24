import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

T_ParamItem {
    id: textItem

    displayText: loader.item ? loader.item.text : ""
    tooltipText: loader.item ? loader.item.text : ""
    tooltipVisible: loader.item ? loader.item.contentWidth > loader.item.width : false

    Component {
        id: text_com
        QuiText {
            id: _content

            anchors.fill: parent

            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            text: paramDisplay !== undefined && paramDisplay !== null ? String(paramDisplay) : ""
            verticalAlignment: Text.AlignVCenter
            font: QuiFont.Caption
            color: QuiColor.FontPrimary
        }
    }

    Component {
        id: textinput_com
        TextInput {
            id: textinput

            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            text: paramDisplay !== undefined && paramDisplay !== null ? String(paramDisplay) : ""
            verticalAlignment: Text.AlignVCenter
            color: QuiColor.FontPrimary
            font: QuiFont.Caption

            onEditingFinished: {
                if (paramDisplay !== textinput.text) {
                    valueChanged(textinput.text)
                }
            }
        }
    }

    Loader {
        id: loader
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.fill: parent
        sourceComponent: paramEditable ? textinput_com : text_com
    }
}
