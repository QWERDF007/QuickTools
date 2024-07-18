import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

Rectangle {
    id: footer

    property var activateItem
    property color itemDisableColor: QuickColor.ItemDisabled
    property color toolbarBorderColor: QuickColor.WindowBackground
    property color toolbarColor: QuickColor.White

    signal fitInWindow
    signal sliderMoved(real value)

    color: QuickColor.White
    height: 40
    width: 200

    Rectangle {
        id: toolbar

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        border.color: toolbarBorderColor
        border.width: 2
        color: footer.enabled ? toolbarColor : itemDisableColor
        height: 36
        implicitWidth: childrenRect.width
        radius: 3

        RowLayout {
            anchors.verticalCenter: parent.verticalCenter
            enabled: {
                if (activateItem instanceof QuickScalableImage) {
                    return activateItem.status === Image.Ready;
                }
                return true;
            }
            spacing: 1

            Label {
                id: imageSize

                Layout.leftMargin: 5
                Layout.preferredWidth: 64
                horizontalAlignment: Text.AlignHCenter
                text: {
                    if ((activateItem instanceof Image || activateItem instanceof QuickScalableImage) && activateItem.status === Image.Ready) {
                        var size = activateItem.sourceSize;
                        return size.width + "x" + size.height;
                    }
                    return "";
                }
                verticalAlignment: Text.AlignVCenter

                MouseArea {
                    id: imageSizeMouseArea

                    anchors.fill: parent
                    hoverEnabled: true
                }
                QuickToolTip {
                    delay: 500
                    text: qsTr("图像大小 (宽x高)")
                    visible: imageSizeMouseArea.containsMouse
                }
            }
            QuickToolButton {
                id: zoomoutBtn

                disableColor: itemDisableColor
                icon.source: "/icons/zoomout"
                implicitHeight: 32
                implicitWidth: 32

                onClicked: {
                    slider.decrease();
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.scaleInCenter(slider.value);
                    }
                    footer.sliderMoved(slider.value);
                }
            }
            QuickSlider {
                id: slider

                from: 0.1
                implicitHeight: 12
                implicitWidth: 100
                stepSize: {
                    if (value < 2) {
                        return 0.1;
                    } else if (value < 10) {
                        return 1;
                    } else {
                        return 2;
                    }
                }
                to: 32
                value: {
                    if (activateItem === null || activateItem === undefined) {
                        return 0;
                    } else if (activateItem instanceof QuickScalableImage) {
                        return activateItem.image.scale;
                    } else if (activateItem instanceof Image) {
                        return activateItem.scale;
                    }
                    return 1;
                }

                onMoved: {
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.scaleInCenter(slider.value);
                    }
                    footer.sliderMoved(slider.value);
                }
            }
            QuickToolButton {
                id: zoominBtn

                disableColor: itemDisableColor
                icon.source: "/icons/zoomin"
                implicitHeight: 32
                implicitWidth: 32

                onClicked: {
                    slider.increase();
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.scaleInCenter(slider.value);
                    }
                    footer.sliderMoved(slider.value);
                }
            }
            QuickToolButton {
                id: fitBtn

                disableColor: itemDisableColor
                icon.source: "/icons/aspectratio"
                implicitHeight: 32
                implicitWidth: 32

                onClicked: {
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.fitInView();
                    }
                    footer.fitInWindow();
                }

                QuickToolTip {
                    delay: 500
                    text: qsTr("适应窗口大小")
                    visible: fitBtn.hovered
                }
            }
            Label {
                Layout.preferredWidth: 48
                Layout.rightMargin: 5
                horizontalAlignment: Text.AlignHCenter
                text: (slider.value * 100).toFixed(2) + "%"
                verticalAlignment: Text.AlignVCenter

                MouseArea {
                    id: scaleLabel

                    anchors.fill: parent
                    hoverEnabled: true
                }
                QuickToolTip {
                    delay: 500
                    text: qsTr("缩放比例")
                    visible: scaleLabel.containsMouse
                }
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
