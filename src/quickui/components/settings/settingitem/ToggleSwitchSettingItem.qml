import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_SettingItem {
    id: toggleSwitchSettingItem

    RowLayout {
        anchors.verticalCenter: parent.verticalCenter

        QuickToggleSwitch {
            checked: settingValue
            text: settingValue ? qsTr("on") : qsTr("off")
            textRight: false

            onClicked: {
                toggleSwitchSettingItem.valueChanged(checked);
            }
        }
    }
}
