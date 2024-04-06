import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core

import "../components/header"
import "../components/sidebar"
import "../components/footer"


ApplicationWindow {
    id: window
    width: 1440
    height: 720
    visible: true
    color: active ? QuickColor.WindowActiveBackground : QuickColor.WindowBackground
    property int winId

    default property alias content: container.data

    property QuickTool quicktool
    property InputParams inputParams
    property OutputParams outputParams
    property var activateItem
    property bool enable: true
    property alias squareChecked: _header.squareChecked
    property alias circleChecked: _header.circleChecked
    property alias polygonChecked: _header.polygonChecked
    signal sliderMoved(real value)
    signal fitInWindow

    Connections {
        target: quicktool
        function onStarted() {
            window.enable = false
        }

        function onFinished() {
            window.enable = true
        }
    }

    header: T_CVHeader {
        id: _header
        enabled: window.enable
        width: parent.width
        height: 40
        onStartBtnClicked: {
            if (quicktool === null || quicktool === undefined) {
                return
            }
            quicktool.exec()
        }
    }

    QuickSplitView {
        id: splitView
        enabled: window.enable
        anchors.fill: parent
        LSideBar {
            id: lsidebar
            border.color: window.color
            SplitView.minimumWidth: 256
            SplitView.preferredWidth: 321
            SplitView.fillHeight: true
            inputParams: window.inputParams
            outputParams: window.outputParams
        }
        Item {
            id: container
            SplitView.fillHeight: true
            SplitView.fillWidth: true
        }
    }

    footer: T_CVFooter {
        id: _footer
        enabled: window.enable
        width: parent.width
        height: 40
        activateItem: window.activateItem
        onSliderMoved: function(value) {
            window.sliderMoved(value)
        }
        onFitInWindow: window.fitInWindow()
    }
    onClosing: function(close) {
        QuickApp.closeWindow(window)
    }
}
