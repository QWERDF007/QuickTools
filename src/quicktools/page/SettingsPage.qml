import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core

QuickContentPage {
    id: settingsPage
    ToolSettingsView {
        id: settingsView
        anchors.fill: parent
        settingsModel: GlobalSettings
        buttonFlags: QuickDialogButtonFlag.PositiveButton
    }
    Connections {
        target: GlobalSettings
        function onChangeStarted() {
            settingsView.enabled = false
            settingsBusyIndicator.open()
        }
        function onChangeFinished() {
            settingsView.enabled = true
            settingsBusyIndicator.close()
        }
    }
    QuickPopup { // 运行状态指示器
        id: settingsBusyIndicator
        width: settingsPage.width
        height: settingsPage.height
        anchors.centerIn: Overlay.overlay
        bg.color: "transparent"
        modal: true
        ColumnLayout {
            spacing: 8
            anchors.centerIn: parent
            QuickProgressRing {
                Layout.alignment: Qt.AlignHCenter
            }
            QuickText{
                text: qsTr("修改中...")
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
