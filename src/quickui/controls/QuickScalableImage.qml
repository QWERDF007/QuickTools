import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

QuiScalableImage {
    id: scalableImage

    property color drawingColor: "lightblue"
    property color drawingBorderColor: "magenta"
    property bool drawing: false

    property int shapeType: QuickShape.NoShape
    property var roiItem: roiLoader.item
    signal roiDataChanged(int shapeType, var data)

    // 输出形状, 显示区域的 model
    property alias shapes: _shapes.model

    onSourceChanged: {
        if (roiItem)
            roiItem.clear()
    }

    Component {
        id: roi_rect
        QuickEditableRect {
            color: scalableImage.drawingColor
            border.color: scalableImage.drawingBorderColor
            border.width: selected ? (scalableImage.image.scale < 0.5 ? 3 : 2) : (scalableImage.image.scale < 0.5 ? 2 : 1)
            visible: false
        }
    }

    Component {
        id: roi_circle
        QuickEditableCircle {
            color: scalableImage.drawingColor
            border.color: scalableImage.drawingBorderColor
            border.width: selected ? (scalableImage.image.scale < 0.5 ? 3 : 2) : (scalableImage.image.scale < 0.5 ? 2 : 1)
            visible: false
        }
    }

    QuickShapeRepeater {
        id: _shapes
    }

    Loader {
        id: roiLoader
        anchors.fill: parent
    }

    Connections {
        target: roiItem
        function onRoiDataChanged() {
            scalableImage.roiDataChanged(roiItem.shapeType, roiItem.roiData)
        }
    }

    onShapeTypeChanged: {
        if (scalableImage.shapeType === QuickShape.Rectangle && roiLoader.sourceComponent !== roi_rect) {
            roiLoader.sourceComponent = roi_rect
            scalableImage.roiDataChanged(QuickShape.NoShape, [])
        } else if (scalableImage.shapeType === QuickShape.Circle && roiLoader.sourceComponent !== roi_circle) {
            scalableImage.roiDataChanged(QuickShape.NoShape, [])
            roiLoader.sourceComponent = roi_circle
        } else if (scalableImage.shapeType === QuickShape.NoShape) {
            roiLoader.sourceComponent = undefined
        }
    }

    MouseArea {
        id: roiMouseArea
        anchors.fill: parent
        enabled: scalableImage.shapeType !== QuickShape.NoShape && scalableImage.status === Image.Ready
        acceptedButtons: Qt.LeftButton
        cursorShape: scalableImage.shapeType !== QuickShape.NoShape ? Qt.CrossCursor : Qt.ArrowCursor

        onPressed: function (mouse) {
            scalableImage.forceActiveFocus()
            scalableImage.drawing = true
            if (roiItem) {
                roiItem.selected = false
                roiItem.startPoint = mapToItem(scalableImage.image, mouse.x, mouse.y)
            }
        }

        onReleased: function (mouse) {
            if (scalableImage.drawing) {
                if (roiItem) {
                    roiItem.updateByPos(mapToItem(scalableImage.image, mouse.x, mouse.y))
                }
                scalableImage.drawing = false
            }
        }

        onPositionChanged: function (mouse) {
            if (scalableImage.drawing && roiItem) {
                roiItem.updateByPos(mapToItem(scalableImage.image, mouse.x, mouse.y))
            }
        }
    }

    Keys.onPressed: function (event) {
        if (event.key === Qt.Key_Delete) {
            if (roiItem) {
                roiItem.clear()
            }
        }
    }
}
