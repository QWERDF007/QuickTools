import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

T_SettingItem {
    id: checkBoxSettingItem

    QuiCheckBox {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        checked: settingValue !== undefined && settingValue !== null ? Boolean(settingValue) : false

        onClicked: {
            valueChanged(checked);
        }
    }
}
