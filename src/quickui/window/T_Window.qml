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
    property bool enable: true

    Connections {
        target: quicktool
        function onStart() {
            busyIndicator.open()
            window.enable = false
        }

        function onStarted() {

        }

        function onFinished() {
            window.enable = true
            busyIndicator.close()
        }

        function onShowMessage(level, msg) {
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
        QuickApp.closeWindow(window)
    }

    QuickPopup {
        id: busyIndicator
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

    QuickLoader {
        property string loadingText
        property bool cancel: false
        id:loader_loading
        anchors.fill: parent
    }

    QuickInfoBar {
        id: infobar
        root: window
    }

    Shortcut {
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
