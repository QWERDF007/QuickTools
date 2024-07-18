import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_SettingItem {
    id: toggleSwitchSettingItem

    QuickToggleSwitch {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        checked: settingValue
        text: settingValue ? qsTr("开") : qsTr("关")
        textRight: false

        onClicked: {
            valueChanged(checked)
        }
    }
}
