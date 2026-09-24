import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui
import QuickTools.core
import quickui

import "../global"

QuiScrollablePage {
    Component { // 工具卡片
        id:com_item
        Item{
            property string desc: modelData.desc
            width: 320
            height: 120
            QuiCard {
                id: card
                width: 300
                height: 100
                anchors.centerIn: parent
                hoverEnabled: true

                Image {
                    id: item_icon
                    height: 40
                    width: 40
                    source: modelData.icon
                    anchors {
                        left: parent.left
                        leftMargin: 20
                        verticalCenter: parent.verticalCenter
                    }
                }

                QuiText {
                    id: item_title
                    text: modelData.title
                    font: QuiFont.BodyStrong
                    color: QuiColor.FontPrimary
                    anchors {
                        left: item_icon.right
                        leftMargin: 20
                        top: item_icon.top
                    }
                }

                QuiText {
                    id: item_desc
                    text: desc
                    color: QuiColor.FontDark
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                    font: QuiFont.Caption
                    maximumLineCount: 2
                    anchors {
                        left: item_title.left
                        right: parent.right
                        rightMargin: 20
                        top: item_title.bottom
                        topMargin: 5
                    }
                }

                onDoubleClicked: {
                    QuickToolsItems.startPageByItem(modelData)
                    var args = {icon: modelData.icon}
                    App.navigate(modelData.url, args)
                }

                QuiToolTip {
                    text: qsTr("双击打开工具")
                    x: card.mouseX
                    y: card.mouseY + 20
                    delay: 500
                    visible: card.hovered
                }
            }
        }
    }

    QuiText{
        text: qsTr("最近添加的工具")
        font: QuiFont.Title
        // Layout.topMargin: 20
        Layout.leftMargin: 20
    }


    GridView{
        id: recentlyAddedToolsView
        Layout.fillWidth: true
        Layout.preferredHeight: contentHeight
        cellHeight: 120
        cellWidth: 320
        model: QuickToolManager.getRecentlyAddedToolsConfig()
        interactive: false
        delegate: com_item
    }

    QuiText{
        text: qsTr("最近更新的工具")
        font: QuiFont.Title
        Layout.topMargin: 20
        Layout.leftMargin: 20
    }

    GridView{
        id: recentlyUpdatedToolsView
        Layout.fillWidth: true
        Layout.preferredHeight: contentHeight
        cellHeight: 120
        cellWidth: 320
        interactive: false
        model: QuickToolManager.getRecentlyUpdatedToolsConfig()
        delegate: com_item
    }
}
