import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import quickui
import QuickTools.ui

T_SettingItem {
    id: colorSettingItem

    RowLayout {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        QuiText {
            Layout.minimumWidth: 64
            horizontalAlignment: Text.AlignRight
            text: settingValue !== undefined && settingValue !== null ? String(settingValue) : ""
            font: QuiFont.Caption
            color: QuiColor.FontPrimary
        }
        Rectangle {
            id: colorRect

            color: mouseArea.containsMouse ? Qt.lighter(settingValue, 1.2) : settingValue
            height: 24
            radius: 4
            width: 24
            border.color: QuiColor.Border
            border.width: 1

            QuiToolTip {
                delay: 500
                text: qsTr("点击修改颜色")
                visible: mouseArea.containsMouse
            }
            MouseArea {
                id: mouseArea

                anchors.fill: parent
                hoverEnabled: true

                onClicked: colorDialog.open()
            }
        }
    }
    ColorDialog {
        id: colorDialog

        currentColor: settingValue

        onAccepted: {
            valueChanged(colorDialog.color)
        }
    }
}
