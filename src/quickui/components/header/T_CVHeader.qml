import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Rectangle {
    id: header
    width: 200
    height: 40
    
    property string toolTitle: qsTr("工具名称")
    property alias squareChecked: squareBtn.checked
    property alias circleChecked: circleBtn.checked
    property alias polygonChecked: polygonBtn.checked
    property color toolbarColor: QuickColor.White
    property color toolbarBorderColor: QuickColor.WindowBackground
    property color itemDisableColor: QuickColor.Grey50
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
        color: header.enabled ? toolbarColor : itemDisableColor
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
            QuickToolButton {
                implicitWidth: 32
                implicitHeight: 32
                icon.source: "/icons/play"
                onClicked: {
                    startBtnClicked()
                }
            }
            Rectangle {
                color: toolbarBorderColor
                width: 3
                Layout.fillHeight: true
            }

            RowLayout {
                id: btnRowLayout
                spacing: 0
                QuickToolButton {
                    id: squareBtn
                    enabled: header.enabled
                    checkable: true
                    implicitWidth: 32
                    implicitHeight: 32
                    icon.source: "/icons/square"
                    onToggled: {
                        circleBtn.checked = false
                        polygonBtn.checked = false
                    }
                }
                QuickToolButton {
                    id: circleBtn
                    enabled: header.enabled
                    checkable: true
                    implicitWidth: 32
                    implicitHeight: 32
                    icon.source: "/icons/circle"
                    onToggled: {
                        squareBtn.checked = false
                        polygonBtn.checked = false
                    }
                }
                QuickToolButton {
                    id: polygonBtn
                    enabled: header.enabled
                    checkable: true
                    implicitWidth: 32
                    implicitHeight: 32
                    icon.source: "/icons/polygon"
                    onToggled: {
                        squareBtn.checked = false
                        circleBtn.checked = false
                    }
                }
            }
        }
    }
}
