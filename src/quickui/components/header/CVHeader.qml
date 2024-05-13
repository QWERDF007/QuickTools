import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

T_Header {
    property var activateItem
    property int acceptedShapes: QuickShape.AllShapes
    property int checkedShape: QuickShape.NoShape

    toolbarButtons: Component {
        RowLayout {
            spacing: 0
            QuickToolButton {
                id: rectangleBtn
                enabled: header.acceptedShapes & QuickShape.Rectangle
                checkable: true
                implicitWidth: 32
                implicitHeight: 32
                disableColor: itemDisableColor
                icon.source: "/icons/square"
                onToggled: {
                    circleBtn.checked = false
                    polygonBtn.checked = false
                    checkedShape = checked ? QuickShape.Rectangle : QuickShape.NoShape
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.shapeType = checked ? QuickShape.Rectangle : QuickShape.NoShape
                    }
                }
            }

            QuickToolButton {
                id: circleBtn
                enabled: header.acceptedShapes & QuickShape.Circle
                checkable: true
                implicitWidth: 32
                implicitHeight: 32
                disableColor: itemDisableColor
                icon.source: "/icons/circle"
                onToggled: {
                    rectangleBtn.checked = false
                    polygonBtn.checked = false
                    checkedShape = checked ? QuickShape.Circle : QuickShape.NoShape
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.shapeType = checked ? QuickShape.Circle : QuickShape.NoShape
                    }
                }
            }

            QuickToolButton {
                id: polygonBtn
                enabled: header.acceptedShapes & QuickShape.Polygon
                checkable: true
                implicitWidth: 32
                implicitHeight: 32
                disableColor: itemDisableColor
                icon.source: "/icons/polygon"
                onToggled: {
                    rectangleBtn.checked = false
                    circleBtn.checked = false
                    checkedShape = checked ? QuickShape.Polygon : QuickShape.NoShape
                    if (activateItem instanceof QuickScalableImage) {
                        activateItem.shapeType = checked ? QuickShape.Polygon : QuickShape.NoShape
                    }
                }
            }
        }
    }
}
