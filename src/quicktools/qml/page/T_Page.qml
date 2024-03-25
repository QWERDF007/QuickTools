import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

import "../component"

QuickScrollablePage {
    id: page
    property ToolsListItem items

    Item {
        id: d
        function handleItems() {
            var data = []
            if (items) {
                for (var i=0;i<items.children.length;i++) {
                    var item = items.children[i]
                    data.push(item)
                }
            }
            return data
        }
    }

    Component {
        id:com_item
        Item{
            property string desc: modelData.desc
            width: 320
            height: 120
            QuickArea { // 带颜色的卡片区域
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
                    source: modelData.image
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
                        console.log("单击", modelData.title)
                    }
                    onDoubleClicked: {
                        console.log("打开对话框", modelData.title, modelData.url)
                        var component =  Qt.createComponent(modelData.url)
                        var window = component.createObject()
                        window.show()
                    }
                }
            }
        }
    }



    GridView {
        id: gridview
        Layout.fillWidth: true
        Layout.preferredHeight: contentHeight
//        anchors.fill: parent
        model: d.handleItems()
        cellHeight: 120
        cellWidth: 320
        interactive: false
        delegate: com_item
    }

    QuickText {
        visible: items == null
        Layout.fillHeight: true
        Layout.fillWidth: true
        text: qsTr("这个人很懒, 啥也没做")
        font.pixelSize: 32
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}

