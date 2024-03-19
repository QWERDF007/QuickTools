import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// ApplicationWindow {
//     id: window
//     visible: true

//     height: 1920
//     width: 1080

//     header: T_CVHeader {
//         color: "red"
//         implicitWidth: window.width
//         implicitHeight: 32
//     }

//     Rectangle {
//         color: "red"
//     }
// }



Rectangle {
    id: window
    height: 1080
    width: 1440
    visible: true
    color: "#EDEDED"

    // T_CVHeader {
    //     anchors.top: parent.top
    //     height: 32
    //     width: parent.width
    // }




    Rectangle {
        id: header
        color: "white"
        border.width: 1
        border.color: "#EDEDED"
        width: parent.width
        height: 40
        anchors.top: parent.top

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
                    icon.source: "file:///F:/workspace/QuickTools/assets/icons/roundedplay.svg"
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
                        icon.source: "file:///F:/workspace/QuickTools/assets/icons/square.svg"
                        palette{
                            button: "white"
                        }
                    }
                    ToolButton {
                        implicitWidth: 32
                        implicitHeight: 32
                        icon.source: "file:///F:/workspace/QuickTools/assets/icons/circle.svg"
                        palette{
                            button: "white"
                        }
                    }
                    ToolButton {
                        implicitWidth: 32
                        implicitHeight: 32
                        icon.source: "file:///F:/workspace/QuickTools/assets/icons/pentagon.svg"
                        palette{
                            button: "white"
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: content
        anchors.top: header.bottom
        anchors.bottom: footer.top
        width: parent.width
    }

    Rectangle {
        id: footer
        color: "white"
        border.width: 1
        border.color: "#EDEDED"
        anchors.bottom: parent.bottom
        width: parent.width
        height: 40

        Rectangle {
            id: footerToolBar
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
                Label {
                    Layout.leftMargin: 5
                    Layout.preferredWidth: 64
                    text: "1024x640"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                ToolButton {
                    implicitWidth: 32
                    implicitHeight: 32
                    icon.source: "file:///F:/workspace/QuickTools/assets/icons/zoomout.svg"
                    palette{
                        button: "white"
                    }
                }
                Slider {
                    id: slider
                    implicitHeight: 12
                    implicitWidth: 100
                    from: 0
                    to: 1
                }

                ToolButton {
                    implicitWidth: 32
                    implicitHeight: 32
                    icon.source: "file:///F:/workspace/QuickTools/assets/icons/zoomin.svg"
                    palette{
                        button: "white"
                    }
                }
                Label {
                    Layout.rightMargin: 5
                    Layout.preferredWidth: 48
                    text: (slider.value * 100).toFixed(2) + "%"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

}
