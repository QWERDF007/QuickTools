import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

ListView {
    id: ltoolListView

    boundsBehavior: Flickable.StopAtBounds
    height: 200
    width: 64

    delegate: Rectangle {
        color: {
            if (!ltoolListView.enabled) {
                return QuiColor.ItemDisabled;
            } else if (mouseArea.pressed) {
                return QuiColor.ItemPress;
            } else if (mouseArea.hovered) {
                return QuiColor.ItemHover;
            } else if (ltoolListView.currentIndex === model.index) {
                return QuiColor.ItemCheck;
            }
            return QuiColor.ItemNormal;
        }
        height: 64
        radius: 4
        width: 64

        Column {
            anchors.bottomMargin: 5
            anchors.fill: parent
            anchors.topMargin: 5
            spacing: 4

            IconLabel {
                anchors.horizontalCenter: parent.horizontalCenter
                icon.height: 32
                icon.source: model.icon
                icon.width: 32 // 工具图标
                implicitHeight: 32
                implicitWidth: 32
                rotation: model.rotation
            }
            QuiText {
                color: ltoolListView.currentIndex === model.index ? QuiColor.Highlight : QuiColor.FontPrimary
                horizontalAlignment: Text.AlignHCenter // 工具标题
                text: model.text
                visible: true
                width: parent.width
                font: QuiFont.Caption
            }
        }
        MouseArea {
            id: mouseArea

            property bool hovered: mouseArea.containsMouse

            acceptedButtons: Qt.LeftButton
            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                ltoolListView.currentIndex = index;
            }
        }
    }
    model: ListModel {
        ListElement {
            icon: "/icons/input"
            rotation: 0
            text: "输入参数"
        }
        ListElement {
            icon: "/icons/output"
            rotation: 180
            text: "输出参数"
        }
        ListElement {
            icon: "/icons/help"
            rotation: 0
            text: "帮助"
        }
    }
}
