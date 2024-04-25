import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Rectangle {
    id: header
    width: 200
    height: 40
    
    property var activateItem
    property bool rectangleEnable: true
    property bool circleEnable: true
    property bool polygonEnable: true
    property alias rectangleChecked: rectangleBtn.checked
    property alias circleChecked: circleBtn.checked
    property alias polygonChecked: polygonBtn.checked
    property color toolbarColor: QuickColor.White
    property color toolbarBorderColor: QuickColor.WindowBackground
    property color itemDisableColor: QuickColor.ItemDisabled
    signal startBtnClicked

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
                id: playBtn
                implicitWidth: 32
                implicitHeight: 32
                disableColor: itemDisableColor
                icon.source: "/icons/play"
                onClicked: startBtnClicked()

                QuickToolTip {
                    visible: playBtn.hovered
                    text: qsTr("Run F5")
                    delay: 500
                }
            }
            Rectangle { // 分割线
                color: toolbarBorderColor
                width: 3
                Layout.fillHeight: true
            }

            RowLayout {
                id: btnRowLayout
                spacing: 0
                QuickToolButton {
                    id: rectangleBtn
                    enabled: header.rectangleEnable
                    checkable: true
                    implicitWidth: 32
                    implicitHeight: 32
                    disableColor: itemDisableColor
                    icon.source: "/icons/square"
                    onToggled: {
                        circleBtn.checked = false
                        polygonBtn.checked = false
                        if (activateItem instanceof QuickScalableImage) {
                            activateItem.shapeType = checked ? QuickShape.Rectangle : QuickShape.NoShape
                        }
                    }
                }
                QuickToolButton {
                    id: circleBtn
                    enabled: header.circleEnable
                    checkable: true
                    implicitWidth: 32
                    implicitHeight: 32
                    disableColor: itemDisableColor
                    icon.source: "/icons/circle"
                    onToggled: {
                        rectangleBtn.checked = false
                        polygonBtn.checked = false
                        if (activateItem instanceof QuickScalableImage) {
                            activateItem.shapeType = checked ? QuickShape.Circle : QuickShape.NoShape
                        }
                    }
                }
                QuickToolButton {
                    id: polygonBtn
                    enabled: header.polygonEnable
                    checkable: true
                    implicitWidth: 32
                    implicitHeight: 32
                    disableColor: itemDisableColor
                    icon.source: "/icons/polygon"
                    onToggled: {
                        rectangleBtn.checked = false
                        circleBtn.checked = false
                        if (activateItem instanceof QuickScalableImage) {
                            activateItem.shapeType = checked ? QuickShape.Polygon : QuickShape.NoShape
                        }
                    }
                }
            }
        }
    }
}
