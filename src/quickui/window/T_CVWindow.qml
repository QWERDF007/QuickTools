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

    property InputParams inputParams
    property OutputParams outputParams

    header: T_CVHeader {
        width: parent.width
        height: 40
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
        width: parent.width
        height: 40
    }
}
