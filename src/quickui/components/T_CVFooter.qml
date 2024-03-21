import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Rectangle {
    id: footer
    color: "white"
    width: 200
    height: 40

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
