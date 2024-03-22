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

    header: T_CVHeader {
        width: parent.width
        height: 40
    }

    SplitView {
        id: splitView
        anchors.fill: parent
        handle: Rectangle {
            id: handleDelegate
            implicitWidth: 4
            implicitHeight: splitView.height
            color: SplitHandle.pressed ?  Qt.darker(Qt.rgba(226/255,229/255,234/255,1), 1.2)
                                       : (SplitHandle.hovered ?  Qt.darker(Qt.rgba(226/255,229/255,234/255,1), 1.1) : Qt.rgba(226/255,229/255,234/255,1))
            Rectangle {
                implicitWidth: 2
                implicitHeight: 12
                anchors.centerIn: parent
            }
            containmentMask: Item {
                x: (handleDelegate.width - width) / 2
                width: 64
                height: splitView.height
            }
        }
        T_CVLSidebar {
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
