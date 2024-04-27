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
    property color itemDisableColor: QuickColor.ItemDisabled
    signal sliderMoved(real value)
    signal fitInWindow

    Rectangle {
        id: toolbar
        color: footer.enabled ? toolbarColor : itemDisableColor
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
                    if ((activateItem instanceof Image || activateItem instanceof QuickScalableImage) && activateItem.status === Image.Ready) {
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
                QuickToolTip{
                    text: qsTr("图像大小 (宽x高)")
                    delay: 500
                    visible: imageSizeMouseArea.containsMouse
                }
            }
            QuickToolButton {
                id: zoomoutBtn
                implicitWidth: 32
                implicitHeight: 32
                disableColor: itemDisableColor
                icon.source: "/icons/zoomout"
                onClicked: {
                    slider.decrease()
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.scaleInCenter(slider.value)
                    }
                    footer.sliderMoved(slider.value)
                }
            }
            Slider {
                id: slider
                implicitHeight: 12
                implicitWidth: 100
                from: 0.1
                value: {
                    if (activateItem === null || activateItem === undefined) {
                        return 0
                    } else if (activateItem instanceof QuickScalableImage) {
                        return activateItem.image.scale
                    } else if (activateItem instanceof Image) {
                        return activateItem.scale
                    }
                    return 1
                }
                stepSize: {
                    if (value < 2) {
                        return 0.1
                    } else if (value < 10) {
                        return 1
                    } else {
                        return 2
                    }
                }

                to: 32
                onMoved: {
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.scaleInCenter(slider.value)
                    }
                    footer.sliderMoved(slider.value)
                }
            }

            QuickToolButton {
                id: zoominBtn
                implicitWidth: 32
                implicitHeight: 32
                disableColor: itemDisableColor
                icon.source: "/icons/zoomin"
                onClicked: {
                    slider.increase()
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.scaleInCenter(slider.value)
                    }
                    footer.sliderMoved(slider.value)
                }
            }
            QuickToolButton {
                id: fitBtn
                implicitWidth: 32
                implicitHeight: 32
                disableColor: itemDisableColor
                icon.source: "/icons/aspectratio"
                onClicked: {
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.fitInView()
                    }
                    footer.fitInWindow()
                }

                QuickToolTip {
                    text: qsTr("适应窗口大小")
                    visible: fitBtn.hovered
                    delay: 500
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

    Shortcut {
        enabled: footer.enabled && activateItem !== null
        sequence: StandardKey.ZoomIn
        onActivated: zoominBtn.clicked()
    }

    Shortcut {
        enabled: footer.enabled && activateItem !== null
        sequence: StandardKey.ZoomOut
        onActivated: zoomoutBtn.clicked()
    }
}