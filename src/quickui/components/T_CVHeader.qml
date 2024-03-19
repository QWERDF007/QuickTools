import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "white"
    border.width: 1
    border.color: "#EDEDED"
    height: 32
    width: 200

    Label {
        text: qsTr("工具名称")
        font.pixelSize: 20
        verticalAlignment: Text.AlignVCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
    }
}
