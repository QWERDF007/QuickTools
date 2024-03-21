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
            width: 64 + 256
            height: parent.height
            Row {
                anchors.fill: parent
            ListView {
                id: lv
                width: 64
                height: parent.height
                boundsBehavior: Flickable.StopAtBounds
                // spacing: 0
                model: ListModel {
                    ListElement {text: "输入参数"; icon: "/icons/input"; rotation: 0 }
                    ListElement {text: "输出参数"; icon: "/icons/output"; rotation: 180 }
                    ListElement {text: "帮助"; icon: "/icons/input"; rotation: 0 }
                }

                delegate: Rectangle {
                    width: 64
                    height: 64
                    color: {
                        if (mouseArea.pressed) {
                            return Qt.rgba(0,0,0,0.06)
                        }  else if (mouseArea.containsMouse) {
                            return Qt.rgba(0,0,0,0.03)
                        } else if (lv.currentIndex === model.index) {
                            return Qt.rgba(0,0,0,0.09) // 222
                        }
                        return Qt.rgba(0,0,0,0) // white
                    }

                    radius: 3
                    Column {
                        anchors.fill: parent
                        anchors.topMargin: 5
                        anchors.bottomMargin: 5
                        spacing: 4
                        IconLabel {
                            implicitHeight: 32
                            implicitWidth: 32
                            anchors.horizontalCenter: parent.horizontalCenter
                            icon.source: model.icon
                            icon.width: 32
                            icon.height: 32
                            rotation: model.rotation
                        }
                        Label {
                            text: model.text
                            width: parent.width
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                    MouseArea {
                        id: mouseArea
                        acceptedButtons: Qt.LeftButton
                        property bool hoverd: mouseArea.containsMouse
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            lv.currentIndex = index
                        }
                    }
                }
            }
            Rectangle {
                border.color: "#EDEDED"
                border.width: 1
                width: 256
                height: parent.height
                StackLayout {
                    anchors.fill: parent
                    currentIndex: lv.currentIndex
                    Item {
                        id: inputParamsArea
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        ListView {
                            id: inputParamsListView
                            anchors.fill: parent
                            anchors.topMargin: 5
                            anchors.bottomMargin: 5
                            boundsBehavior: Flickable.StopAtBounds
                            model: 3
                            delegate: Item {
                                width: inputParamsListView.width
                                height: 48
                                Rectangle {
                                    width: 200
                                    height: 40
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    color: "white"
                                    border.width: 1
                                    border.color: "#EDEDED"
                                }
                            }
                        }
                    }
                    Item {
                        id: outputParamsArea
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        ListView {
                            id: outputParamsListView
                            anchors.fill: parent
                            anchors.topMargin: 5
                            anchors.bottomMargin: 5
                            boundsBehavior: Flickable.StopAtBounds
                            model: 5
                            delegate: Item {
                                width: outputParamsListView.width
                                height: 48
                                //                            color: "red"
                                Rectangle {
                                    width: 200
                                    height: 40
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    color: "white"
                                    border.width: 1
                                    border.color: "#EDEDED"
                                }
                            }
                        }
                    }
                    Item {
                        id: helpInfos
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
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
