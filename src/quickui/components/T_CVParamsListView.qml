import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: 200
    height: 400
    property alias model: paramsListView.model

    ListView {
        id: paramsListView
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        boundsBehavior: Flickable.StopAtBounds
        delegate: Item {
            width: paramsListView.width
            height: 48
            Rectangle {
                height: 40
                anchors {
                    left: parent.left
                    leftMargin: 10
                    right: parent.right
                    rightMargin: 10
                }
                color: "white"
                border.width: 1
                border.color: "#EDEDED"
            }
        }
    }
}
