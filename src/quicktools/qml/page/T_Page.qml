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
            QuickArea {
                radius: 8
                width: 300
                height: 100
                anchors.centerIn: parent
                Rectangle{
                    anchors.fill: parent
                    radius: 8
                    color:{
                        if(item_mouse.containsMouse){
                            return QuickColor.ItemHover
                        }
                        return QuickColor.ItemNormal
                    }
                }
                Image{
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
                QuickText{
                    id:item_title
                    text:modelData.title
                    font: QuickFont.BodyStrong
                    anchors{
                        left: item_icon.right
                        leftMargin: 20
                        top: item_icon.top
                    }
                }
                QuickText{
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


}

