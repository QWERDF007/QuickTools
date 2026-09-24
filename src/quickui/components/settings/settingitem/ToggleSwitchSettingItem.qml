import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

T_SettingItem {
    id: toggleSwitchSettingItem

    QuiToggleSwitch {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        checked: settingValue !== undefined && settingValue !== null ? Boolean(settingValue) : false
        text: checked ? qsTr("开") : qsTr("关")
        textRight: false

        onClicked: {
            valueChanged(checked)
        }
    }
}
