import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui
import QuickTools.core

T_Header {
    property int acceptedShapes: ShapeType.AllShapes
    property var activateItem
    property int checkedShape: ShapeType.NoShape

    toolbarButtons: Component {
        RowLayout {
            spacing: 0

            QuickToolButton {
                id: rectangleBtn

                checkable: true
                disableColor: itemDisableColor
                enabled: header.acceptedShapes & ShapeType.Rectangle
                icon.color: checked ? "#009688" : palette.buttonText
                icon.source: "/icons/square"
                implicitHeight: 32
                implicitWidth: 32

                onToggled: {
                    circleBtn.checked = false;
                    polygonBtn.checked = false;
                    checkedShape = checked ? ShapeType.Rectangle : ShapeType.NoShape;
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.shapeType = checked ? ShapeType.Rectangle : ShapeType.NoShape;
                        console.log("activateItem.shapeType", activateItem.shapeType)
                    }
                }
            }
            QuickToolButton {
                id: circleBtn

                checkable: true
                disableColor: itemDisableColor
                enabled: header.acceptedShapes & ShapeType.Circle
                icon.color: checked ? "#009688" : palette.buttonText
                icon.source: "/icons/circle"
                implicitHeight: 32
                implicitWidth: 32

                onToggled: {
                    rectangleBtn.checked = false;
                    polygonBtn.checked = false;
                    checkedShape = checked ? ShapeType.Circle : ShapeType.NoShape;
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.shapeType = checked ? ShapeType.Circle : ShapeType.NoShape;
                    }
                }
            }
            QuickToolButton {
                id: polygonBtn

                checkable: true
                disableColor: itemDisableColor
                enabled: header.acceptedShapes & ShapeType.Polygon
                icon.color: checked ? "#009688" : palette.buttonText
                icon.source: "/icons/polygon"
                implicitHeight: 32
                implicitWidth: 32

                onToggled: {
                    rectangleBtn.checked = false;
                    circleBtn.checked = false;
                    checkedShape = checked ? ShapeType.Polygon : ShapeType.NoShape;
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.shapeType = checked ? ShapeType.Polygon : ShapeType.NoShape;
                    }
                }
            }
        }
    }
}
