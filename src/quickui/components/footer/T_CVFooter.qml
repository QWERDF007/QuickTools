import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Rectangle {
    id: footer
    color: QuickColor.White
    width: 200
    height: 40

    property var activateItem
    property color toolbarColor: QuickColor.White
    property color toolbarBorderColor: QuickColor.WindowBackground

    Rectangle {
        id: toolbar
        color: toolbarColor
        radius: 3
        border.width: 2
        border.color: toolbarBorderColor
        height: 36
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        implicitWidth: childrenRect.width
        RowLayout {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 1
            Label {
                id: imageSize
                Layout.leftMargin: 5
                Layout.preferredWidth: 64
                text: {
                    if (activateItem instanceof Image && activateItem.status === Image.Ready) {
                        var size = activateItem.sourceSize
                        return size.width + "x" + size.height
                    }
                    return ""
                }
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                MouseArea {
                    id: imageSizeMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                }
                ToolTip{
                    text: qsTr("图像大小 (宽x高)")
                    delay: 500
                    visible: imageSizeMouseArea.containsMouse
                }
            }
            ToolButton {
                implicitWidth: 32
                implicitHeight: 32
                icon.source: "/icons/zoomout"
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
                icon.source: "/icons/zoomin"
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
