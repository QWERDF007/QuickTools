import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "red"
    Rectangle {
        id: r1
        color: "blue"
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        width: parent.width / 2
    }
    Rectangle {
        id: r2
        color: "green"
        anchors {
            left: r1.right
            top: parent.top
            bottom: parent.bottom
        }
        width: parent.width / 2
    }
}
