import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_SettingItem {
    id: checkBoxSettingItem

    CheckBox {
        anchors.verticalCenter: parent.verticalCenter
        checked: settingValue

        onToggled: {
            valueChanged(checked);
        }
    }
}
