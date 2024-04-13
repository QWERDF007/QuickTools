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
            indicator.open()
            window.enable = false
        }

        function onStarted() {

        }

        function onFinished() {
            window.enable = true
            indicator.close()
        }
    }

    onClosing: function(close) {
        QuickApp.closeWindow(window)
    }

    QuickPopup {
        id: indicator
        bg.color: "transparent"
        modal: true
        QuickProgressRing {
            // strokeWidth: 0
        }
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
