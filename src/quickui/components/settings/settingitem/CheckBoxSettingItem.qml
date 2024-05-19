import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_SettingItem {
    id: checkBoxSettingItem

    RowLayout {
        anchors.verticalCenter: parent.verticalCenter

        CheckBox {
            checked: settingValue

            onToggled: {
                checkBoxSettingItem.valueChanged(checked);
            }
        }
    }
}
