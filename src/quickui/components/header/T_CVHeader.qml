import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Rectangle {
    id: header
    width: 200
    height: 40

    property string toolTitle: qsTr("工具名称")
    signal startBtnClicked

    QuickText {
        text: toolTitle
        font: QuickFont.Subtitle
        verticalAlignment: Text.AlignVCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle {
        id: toolbar
        color: "white"
        radius: 3
        border.width: 2
        border.color: "#EDEDED"
        height: 36
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        implicitWidth: childrenRect.width
        RowLayout {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 1
            ToolButton {
                implicitWidth: 32
                implicitHeight: 32
                icon.source: "/icons/play"
                onClicked: {
                    startBtnClicked()
                }
            }
            Rectangle {
                color: "#EDEDED"
                width: 3
                Layout.fillHeight: true
            }

            RowLayout {
                ToolButton {
                    implicitWidth: 32
                    implicitHeight: 32
                    icon.source: "/icons/square"
                }
                ToolButton {
                    implicitWidth: 32
                    implicitHeight: 32
                    icon.source: "/icons/circle"
                }
                ToolButton {
                    implicitWidth: 32
                    implicitHeight: 32
                    icon.source: "/icons/polygon"
                }
            }
        }
    }
}
