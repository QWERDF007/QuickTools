import QtQuick
import QtQuick.Window

Item {
    id:control
    property int orientation: Qt.Horizontal
    property int spacing:0
    property int size: 1

    QtObject{
        id:d
        property bool isVertical : orientation === Qt.Vertical
    }

    width: d.isVertical ? spacing*2+size : parent.width
    height: d.isVertical ? parent.height : spacing*2+size

    Rectangle{
        color: Qt.rgba(210/255,210/255,210/255,1)
        width: d.isVertical ? size : parent.width
        height: d.isVertical ? parent.height : size
        anchors.centerIn: parent
    }
}
