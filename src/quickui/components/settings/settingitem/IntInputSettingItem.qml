import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_SettingItem {
    id: intInputSettingItem

    TextField {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        implicitWidth: Math.max(64, contentWidth + 20)
        implicitHeight: 32
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: settingValue
        onEditingFinished: {
            valueChanged(text)
        }
    }
}
