import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui
import QuickTools.core

import "../global"

QuickScrollablePage {
    Component { // 工具卡片
        id:com_item
        Item{
            property string desc: modelData.desc
            width: 320
            height: 120
            QuickFrame { // 带颜色的卡片区域
                radius: 8
                width: 300
                height: 100
                anchors.centerIn: parent
                Rectangle{ // 背景区域, 控制悬浮时的颜色变化
                    anchors.fill: parent
                    radius: 8
                    color: item_mouse.containsMouse ? QuickColor.ItemHover : QuickColor.ItemNormal
                }

                Image{ // 图标
                    id:item_icon
                    height: 40
                    width: 40
                    source: modelData.icon
                    anchors{
                        left: parent.left
                        leftMargin: 20
                        verticalCenter: parent.verticalCenter
                    }
                }

                QuickText{ // 标题
                    id:item_title
                    text:modelData.title
                    font: QuickFont.BodyStrong
                    anchors{
                        left: item_icon.right
                        leftMargin: 20
                        top: item_icon.top
                    }
                }

                QuickText{ // 工具描述
                    id:item_desc
                    text:desc
                    color: QuickColor.Grey120
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                    font: QuickFont.Caption
                    maximumLineCount: 2
                    anchors{
                        left: item_title.left
                        right: parent.right
                        rightMargin: 20
                        top: item_title.bottom
                        topMargin: 5
                    }
                }

                MouseArea{
                    id:item_mouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                    }
                    onDoubleClicked: {
                        QuickToolsItems.startPageByItem(modelData)
                        var args = {icon: modelData.icon}
                        App.navigate(modelData.url, args)
                    }
                }

                QuickToolTip {
                    text: qsTr("双击打开工具")
                    x: item_mouse.mouseX
                    y: item_mouse.mouseY + 20
                    delay: 500
                    visible: item_mouse.containsMouse
                }
            }
        }
    }

    QuickText{
        text: qsTr("最近添加的工具")
        font: QuickFont.Title
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

    QuickText{
        text: qsTr("最近更新的工具")
        font: QuickFont.Title
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
