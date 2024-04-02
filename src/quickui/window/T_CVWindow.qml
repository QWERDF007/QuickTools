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

    default property alias content: container.data

    property alias inputParams: lsidebar.inputParams
    property alias outputParams: lsidebar.outputParams
    property QuickTool quicktool
    property alias activateItem: _footer.activateItem

    header: T_CVHeader {
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
        anchors.fill: parent
        LSideBar {
            id: lsidebar
            border.color: window.color
            SplitView.minimumWidth: 256
            SplitView.preferredWidth: 321
            SplitView.fillHeight: true
        }
        Item {
            id: container
            SplitView.fillHeight: true
            SplitView.fillWidth: true
        }
    }

    footer: T_CVFooter {
        id: _footer
        width: parent.width
        height: 40
    }
}
