import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
        id: header
//        color: "white"
//        border.width: 1
//        border.color: "#EDEDED"
        width: 200
        height: 40


        Label {
            text: qsTr("工具名称")
            font.pixelSize: 20

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
                    palette{
                        button: "white"
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
                        palette{
                            button: "white"
                        }
                    }
                    ToolButton {
                        implicitWidth: 32
                        implicitHeight: 32
                        icon.source: "/icons/circle"
                        palette{
                            button: "white"
                        }
                    }
                    ToolButton {
                        implicitWidth: 32
                        implicitHeight: 32
                        icon.source: "/icons/polygon"
                        palette{
                            button: "white"
                        }
                    }
                }
            }
        }
    }
