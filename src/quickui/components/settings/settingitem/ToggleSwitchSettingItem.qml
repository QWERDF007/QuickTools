import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_SettingItem {
    id: toggleSwitchSettingItem

    QuickToggleSwitch {
        anchors.verticalCenter: parent.verticalCenter
        checked: settingValue
        text: settingValue ? qsTr("on") : qsTr("off")
        textRight: false

        onClicked: {
            valueChanged(checked)
        }
    }
}
