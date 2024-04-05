import QtQuick
import QtQuick.Controls

SplitView {
    id: splitView
    property int backgroundWidth: splitView.orientation === Qt.Horizontal ? 4 : splitView.width
    property int backgroundHeight: splitView.orientation === Qt.Horizontal ? splitView.height : 4
    property color backgroundColor: Qt.rgba(226/255,229/255,234/255,1)
    property int handleWidth: splitView.orientation === Qt.Horizontal ? 2 : 24
    property int handleHeight: splitView.orientation === Qt.Horizontal ? 24 : 2
    property color handleColor: "white"
    property int maskWidth: splitView.orientation === Qt.Horizontal ? 12 : splitView.width
    property int maskHeight: splitView.orientation === Qt.Horizontal ? splitView.height : 12

    handle: Rectangle {
        id: handleDelegate
        // 设置 width height 无法正常显示
//        width: splitView.backgroundWidth
//        height: splitView.backgroundHeight
        implicitWidth: splitView.backgroundWidth
        implicitHeight: splitView.backgroundHeight
        color: SplitHandle.pressed ? Qt.darker(backgroundColor, 1.2) : (SplitHandle.hovered ? Qt.darker(backgroundColor, 1.1) : backgroundColor)

        Rectangle {
            width: splitView.handleWidth
            height: splitView.handleHeight
            color: splitView.handleColor
            anchors.centerIn: parent
        }
        containmentMask: Item {
            x: (handleDelegate.width - width) / 2
            width: splitView.maskWidth
            height: splitView.maskHeight
        }
    }
}
