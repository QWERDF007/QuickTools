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

    default property alias content: container.data

    property QuickTool quicktool
    property string helpInfos: quicktool ? quicktool.doc : ""
    property QuickToolSettings toolSettings: quicktool ? quicktool.settings : null
    property InputParams inputParams: quicktool ? quicktool.inputParams : null
    property OutputParams outputParams: quicktool ? quicktool.outputParams : null
    property bool hasPython: quicktool ? quicktool.hasOwnProperty("hasPython") && quicktool.hasPython : false
    property real progress: quicktool ? quicktool.progress : 0

    property bool enabled: true

    title: {
        if (quicktool) {
            return quicktool.name
        }
        return "QuickTools"
    }

    header: T_Header {
        height: 40
        width: window.width
        hasPython: window.hasPython
        onSettingsBtnClicked: window.openSettings()
        onStartBtnClicked: window.run()
        onReloadBtnClicked: window.reloadModule()
    }

    Item {
        anchors.fill: parent
        QuickSplitView {
            id: splitView

            anchors.fill: parent

            LSideBar {
                id: lsidebar

                SplitView.fillHeight: true
                SplitView.minimumWidth: 256
                SplitView.preferredWidth: 321
                border.color: window.color
                childrenEnable: window.enabled
                helpInfos: window.helpInfos
                inputParams: window.inputParams
                outputParams: window.outputParams
            }
            Item {
                id: container

                SplitView.fillHeight: true
                SplitView.fillWidth: true
                enabled: window.enabled
            }
        }
        QuickProgressBar { // 工具运行进度条, 运行过程中显示, 完成后消失
            id: progressbar
            visible: false
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            indeterminate: false
            z: 65536
            from: 0
            to: 1
            value: window.progress

            states: [
                State {
                    name: "finished"
                    when: outputParams && outputParams.pdata.Status ? outputParams.pdata.Status[0] !== 0 : false
                    PropertyChanges { target: progressbar; color: "red" }
                }
            ]
        }
    }

    Connections {
        target: quicktool
        function onStart() {
            busyIndicator.open()
            window.enabled = false
        }

        function onStarted() {
            if (!progressbar.visible) {
                progressbar.visible = true
            }
        }

        function onFinished(status) {
            window.enabled = true
            progressbar.visible = false
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

    ToolSettingsDialog {
        id: toolSettingsDialog
        width: 800
        height: 600
        settingsModel: window.toolSettings
    }

    function run() {
        if (quicktool) {
            quicktool.submit()
        }
    }

    function openSettings() {
        toolSettingsDialog.show()
    }

    function reloadModule() {
        if (quicktool && quicktool.hasPython) {
            quicktool.reloadModule()
        }
    }
}
