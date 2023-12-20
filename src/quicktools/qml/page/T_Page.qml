import QtQuick
import QtQuick.Controls

import QuickTools.ui

QuickScrollablePage {

    Component {
        id:com_item
        Item{
            property string desc: model.desc
            width: 320
            height: 120
            QuickArea{
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
                    source: model.image
                    anchors{
                        left: parent.left
                        leftMargin: 20
                        verticalCenter: parent.verticalCenter
                    }
                }
                QuickText{
                    id:item_title
                    text:model.title
//                    font: FluTextStyle.BodyStrong
                    font: QuickFont.Body
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
//                    font: FluTextStyle.Caption
                    font: QuickFont.Body
                    maximumLineCount: 2
                    anchors{
                        left: item_title.left
                        right: parent.right
                        rightMargin: 20
                        top: item_title.bottom
                        topMargin: 5
                    }
                }

                Rectangle{
                    height: 12
                    width: 12
                    radius:  6
                    color: QuickColor.Primary
                    anchors{
                        right: parent.right
                        top: parent.top
                        rightMargin: 14
                        topMargin: 14
                    }
                }

                MouseArea{
                    id:item_mouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
//                        ItemsOriginal.startPageByItem(modelData)
                    }
                }
            }
        }
    }



    GridView {
        Layout.fillWidth: true
        Layout.preferredHeight: contentHeight
//        anchors.fill: parent
        cellHeight: 120
        cellWidth: 320
        interactive: false
        model: ListModel {
            id: testModel
            ListElement {
                desc: "abababa"
                title: "aaaa"
                image: ""
            }
        }
        delegate: com_item
    }

}
