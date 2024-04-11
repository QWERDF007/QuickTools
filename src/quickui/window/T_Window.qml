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
        function onStarted() {
            window.enable = false
        }

        function onFinished() {
            window.enable = true
        }
    }

    onClosing: function(close) {
        QuickApp.closeWindow(window)
    }
}
