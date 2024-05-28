import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_SettingItem {
    id: intInputSettingItem

    RowLayout {
        anchors.verticalCenter: parent.verticalCenter

        TextField {
            text: settingValue
            onEditingFinished: {
                intInputSettingItem.valueChanged(text)
            }
        }
    }
}
