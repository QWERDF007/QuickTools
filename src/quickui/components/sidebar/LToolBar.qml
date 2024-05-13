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
            if (!ltoolListView.enabled) {
                return QuickColor.ItemDisabled
            } else if (mouseArea.pressed) {
                return QuickColor.ItemPress
            }  else if (mouseArea.hovered) {
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
            IconLabel { // 工具图标
                implicitHeight: 32
                implicitWidth: 32
                anchors.horizontalCenter: parent.horizontalCenter
                icon.source: model.icon
                icon.width: 32
                icon.height: 32
                rotation: model.rotation
            }
            QuickText{ // 工具标题
                text: model.text
                visible: true
                width: parent.width
                color: mouseArea.pressed ? QuickColor.Grey120 : QuickColor.Grey220
                horizontalAlignment: Text.AlignHCenter
            }
        }
        MouseArea {
            id: mouseArea
            acceptedButtons: Qt.LeftButton
            property bool hovered: mouseArea.containsMouse
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                ltoolListView.currentIndex = index
            }
        }
    }
}
