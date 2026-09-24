import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core
import quickui

QuiPage {
    id: settingsPage
    ToolSettingsView {
        id: settingsView
        anchors.fill: parent
        settingsModel: GlobalSettings
        useNegativeButton: false
        usePositiveButton: true
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
    QuiPopup { // 运行状态指示器
        id: settingsBusyIndicator
        width: settingsPage.width
        height: settingsPage.height
        anchors.centerIn: Overlay.overlay
        bg.color: "transparent"
        modal: true
        ColumnLayout {
            spacing: 8
            anchors.centerIn: parent
            QuiProgressRing {
                Layout.alignment: Qt.AlignHCenter
            }
            QuiText{
                text: qsTr("修改中...")
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
