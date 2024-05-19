import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui
import Qt.labs.platform

T_SettingItem {
    id: colorSettingItem

    RowLayout {
        anchors.verticalCenter: parent.verticalCenter

        QuickText {
            Layout.minimumWidth: 64
            horizontalAlignment: Text.AlignRight
            text: settingValue
        }
        Rectangle {
            id: colorRect

            color: mouseArea.containsMouse ? Qt.lighter(settingValue, 1.2) : settingValue
            height: 24
            radius: 3
            width: 24

            QuickToolTip {
                delay: 500
                text: qsTr("Click to Change ROI Color")
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
