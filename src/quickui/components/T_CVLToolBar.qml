import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui


ListView {
    id: ltoolListView
    width: 64
    height: 200
    boundsBehavior: Flickable.StopAtBounds
    model: ListModel {
        ListElement {text: "输入参数"; icon: "/icons/input"; rotation: 0 }
        ListElement {text: "输出参数"; icon: "/icons/output"; rotation: 180 }
        ListElement {text: "帮助"; icon: "/icons/help"; rotation: 0 }
    }

    delegate: Rectangle {
        width: 64
        height: 64
        radius: 3
        color: {
            if (mouseArea.pressed) {
                return QuickColor.ItemPress
            }  else if (mouseArea.hoverd) {
                return QuickColor.ItemHover
            } else if (ltoolListView.currentIndex === model.index) {
                return QuickColor.ItemCheck
            }
            return QuickColor.ItemNormal
        }
        Column {
            anchors.fill: parent
            anchors.topMargin: 5
            anchors.bottomMargin: 5
            spacing: 4
            IconLabel {
                implicitHeight: 32
                implicitWidth: 32
                anchors.horizontalCenter: parent.horizontalCenter
                icon.source: model.icon
                icon.width: 32
                icon.height: 32
                rotation: model.rotation
            }
            QuickText{ // 标题文字
                text: model.text
                visible: true
                width: parent.width
                color: {
                    if (mouseArea.pressed){
                        return QuickColor.Grey120
                    }
                    return QuickColor.Grey220
                }
                horizontalAlignment: Text.AlignHCenter
            }
        }
        MouseArea {
            id: mouseArea
            acceptedButtons: Qt.LeftButton
            property bool hoverd: mouseArea.containsMouse
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                ltoolListView.currentIndex = index
            }
        }
    }
}
