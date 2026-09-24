import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

Rectangle {
    id: footer

    property var activateItem
    property color itemDisableColor: QuiColor.ItemDisabled
    property color toolbarBorderColor: QuiColor.WindowBackground
    property color toolbarColor: QuiColor.CardBackground

    signal fitInWindow
    signal sliderMoved(real value)

    color: QuiColor.CardBackground
    height: 40

    QuiZoomBar {
        id: zoomBar
        anchors.fill: parent
        targetItem: footer.activateItem
        toolbarBorderColor: footer.toolbarBorderColor
        toolbarColor: footer.toolbarColor
        itemDisableColor: footer.itemDisableColor
        onFitRequested: footer.fitInWindow()
        onZoomChanged: function (val) {
            footer.sliderMoved(val)
        }
    }

    Shortcut {
        enabled: footer.enabled && activateItem !== null
        sequence: StandardKey.ZoomIn
        onActivated: {
            if (activateItem) {
                var cur = activateItem.image ? activateItem.image.scale : activateItem.scale
                if (typeof activateItem.scaleInCenter === "function") {
                    activateItem.scaleInCenter(cur + 0.1)
                }
            }
        }
    }

    Shortcut {
        enabled: footer.enabled && activateItem !== null
        sequence: StandardKey.ZoomOut
        onActivated: {
            if (activateItem) {
                var cur = activateItem.image ? activateItem.image.scale : activateItem.scale
                if (typeof activateItem.scaleInCenter === "function") {
                    activateItem.scaleInCenter(Math.max(0.1, cur - 0.1))
                }
            }
        }
    }
}
