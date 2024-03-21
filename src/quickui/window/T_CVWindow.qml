import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import "../components"


ApplicationWindow {
    id: window
    width: 1440
    height: 720
    visible: true
    color: active ? QuickColor.WindowActiveBackground : QuickColor.WindowBackground

    header: T_CVHeader {
        width: parent.width
        height: 40
    }

    Rectangle {
        id: content
        color: "transparent"
        anchors.fill: parent

        Rectangle {
            border.width: 1
            border.color: "#EDEDED"
            width: 64 + 1 + 256
            height: parent.height
            Row {
                anchors.fill: parent
                T_CVLToolBar {
                    id: ltoolbar
                    width: 64
                    height: parent.height
                }
                Rectangle {
                    width: 1
                    height: parent.height
                    color: window.color
                }

                T_CVLToolPanel {
                    currentIndex: ltoolbar.currentIndex
                    width: 256
                    height: parent.height

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
            }
        }
    }

    footer: T_CVFooter {
        width: parent.width
        height: 40
    }
}
