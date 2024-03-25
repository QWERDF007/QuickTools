import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

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
    property alias inputParamsModel: lsidebar.inputParamsModel
    property alias outputParamsModel: lsidebar.outputParamsModel

    header: T_CVHeader {
        width: parent.width
        height: 40
    }

    QuickSplitView {
        id: splitView
        anchors.fill: parent
        T_CVLSidebar {
            id: lsidebar
            border.color: window.color
            SplitView.minimumWidth: 256
            SplitView.preferredWidth: 321
            SplitView.fillHeight: true
            inputParamsModel: ListModel {
                ListElement { text: "1"}
                ListElement { text: "2"}
                ListElement { text: "3"}
            }
            outputParamsModel: ListModel {
                ListElement { text: "1"}
                ListElement { text: "2"}
                ListElement { text: "3"}
                ListElement { text: "4"}
            }
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
