import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core

ApplicationWindow {
    id: window
    width: 1440
    height: 720
    visible: true
    color: active ? QuickColor.WindowActiveBackground : QuickColor.WindowBackground

    property QuickTool quicktool
    property string helpInfos: ""
    property bool enabled: true

    title: {
        if (quicktool === null || quicktool === quicktool === undefined) {
            return "QuickTools"
        }
        return quicktool.name
    }

    Connections {
        target: quicktool
        function onStart() {
            busyIndicator.open()
            window.enabled = false
        }

        function onStarted() {

        }

        function onFinished() {
            window.enabled = true
            busyIndicator.close()
        }

        function onShowMessage(level, msg) { // 消息通知
            switch (level) {
            case QuickTool.Info: return infobar.showInfo(msg)
            case QuickTool.Warning: return infobar.showWarning(msg)
            case QuickTool.Error: return infobar.showError(msg)
            case QuickTool.Success: return infobar.showSuccess(msg)
            case QuickTool.Custom: return infobar.showCustom(msg)
            }
        }
    }

    onClosing: function(close) {
        App.closeWindow(window)
    }

    QuickPopup { // 运行状态指示器
        id: busyIndicator
        width: window.width
        height: window.height
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
                text: qsTr("运行中...")
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }

    QuickInfoBar {
        id: infobar
        root: window
    }

    Shortcut { // 运行快捷键
        enabled: window.enabled
        sequence: StandardKey.Refresh
        onActivated: run()
    }

    function run() {
        if (quicktool === null || quicktool === undefined) {
            return
        }
        quicktool.submit()
    }
}
