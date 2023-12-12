import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

ApplicationWindow {
    id: root
    visible: true
    width: 1920
    height: 1080
//    color: "red"

    menuBar: MenuBar {
        implicitHeight: 12
    }

    header: ToolBar {
        implicitHeight: 24
    }

    footer: Rectangle {
        color: "lightgray"
        implicitHeight: 24
    }

    Item {

        anchors.fill: parent
        RowLayout {
            anchors.fill: parent
            ListView {
                Layout.preferredWidth: 320
                Layout.fillHeight: true

                model: ListModel {
                    ListElement { text: "图像处理" }
                    ListElement { text: "深度学习" }
                    ListElement { text: "图像检索" }
                }

                delegate: QuickExpander {
                    headerText: model.text
                    headerHeight: 24
                }
            }
            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "gray"
            }
        }
    }
}
