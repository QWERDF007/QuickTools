import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_Header {
    property int acceptedShapes: QuickShape.AllShapes
    property var activateItem
    property int checkedShape: QuickShape.NoShape

    toolbarButtons: Component {
        RowLayout {
            spacing: 0

            QuickToolButton {
                id: rectangleBtn

                checkable: true
                disableColor: itemDisableColor
                enabled: header.acceptedShapes & QuickShape.Rectangle
                icon.color: checked ? "#009688" : palette.buttonText
                icon.source: "/icons/square"
                implicitHeight: 32
                implicitWidth: 32

                onToggled: {
                    circleBtn.checked = false;
                    polygonBtn.checked = false;
                    checkedShape = checked ? QuickShape.Rectangle : QuickShape.NoShape;
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.shapeType = checked ? QuickShape.Rectangle : QuickShape.NoShape;
                    }
                }
            }
            QuickToolButton {
                id: circleBtn

                checkable: true
                disableColor: itemDisableColor
                enabled: header.acceptedShapes & QuickShape.Circle
                icon.color: checked ? "#009688" : palette.buttonText
                icon.source: "/icons/circle"
                implicitHeight: 32
                implicitWidth: 32

                onToggled: {
                    rectangleBtn.checked = false;
                    polygonBtn.checked = false;
                    checkedShape = checked ? QuickShape.Circle : QuickShape.NoShape;
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.shapeType = checked ? QuickShape.Circle : QuickShape.NoShape;
                    }
                }
            }
            QuickToolButton {
                id: polygonBtn

                checkable: true
                disableColor: itemDisableColor
                enabled: header.acceptedShapes & QuickShape.Polygon
                icon.color: checked ? "#009688" : palette.buttonText
                icon.source: "/icons/polygon"
                implicitHeight: 32
                implicitWidth: 32

                onToggled: {
                    rectangleBtn.checked = false;
                    circleBtn.checked = false;
                    checkedShape = checked ? QuickShape.Polygon : QuickShape.NoShape;
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.shapeType = checked ? QuickShape.Polygon : QuickShape.NoShape;
                    }
                }
            }
        }
    }
}
