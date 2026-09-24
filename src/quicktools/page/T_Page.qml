import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import quickui

QuiScrollablePage { // 工具卡片列表页面, 可滚动
    id: page

    property alias model: gridview.model

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

                QuiText{ // 标题
                    id:item_title
                    text:modelData.title
                    font: QuiFont.BodyStrong
                    color: QuiColor.FontPrimary
                    anchors{
                        left: item_icon.right
                        leftMargin: 20
                        top: item_icon.top
                    }
                }

                QuiText{ // 工具描述
                    id:item_desc
                    text:desc
                    color: QuiColor.FontDark
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                    font: QuiFont.Caption
                    maximumLineCount: 2
                    anchors{
                        left: item_title.left
                        right: parent.right
                        rightMargin: 20
                        top: item_title.bottom
                        topMargin: 5
                    }
                }

                onDoubleClicked: {
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



    GridView {
        id: gridview
        Layout.fillWidth: true
        Layout.preferredHeight: contentHeight
        cellHeight: 120
        cellWidth: 320
        interactive: false
        delegate: com_item
    }

    QuiText {
        visible: gridview.count <= 0
        Layout.fillHeight: true
        Layout.fillWidth: true
        text: qsTr("这个人很懒, 啥也没做")
        font.pixelSize: 32
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}

