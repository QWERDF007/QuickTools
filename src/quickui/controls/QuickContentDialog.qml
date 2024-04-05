import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import QuickTools.ui

QuickPopup {
    id: control
    property string title: ""
    property string message: ""
    property string neutralText: qsTr("关闭")
    property string negativeText: qsTr("取消")
    property string positiveText: qsTr("确认")
    property int messageTextFormart: Text.AutoText
    property int delayTime: 100
    property int buttonFlags: QuickDialogButtonFlag.NegativeButton | QuickDialogButtonFlag.PositiveButton
    property var contentDelegate:  Component{ // 默认内容代理为空, 可重载
        Item{
        }
    }
    property var onNeutralClickListener // 按钮点击监听
    property var onNegativeClickListener
    property var onPositiveClickListener
    signal neutralClicked
    signal negativeClicked
    signal positiveClicked
    implicitWidth: 400
    implicitHeight: layoutContent.height
    focus: true
    Component{
        id:comMessage
        Flickable{
            id:srollMessage
            contentHeight: textMessage.height
            contentWidth: width
            clip: true
            boundsBehavior:Flickable.StopAtBounds
            width: parent.width
            height: message === "" ? 0 : Math.min(textMessage.height,300)
            ScrollBar.vertical: ScrollBar {}
            QuickText { // 消息
                id:textMessage
                font: QuickFont.Body
                wrapMode: Text.WrapAnywhere
                text:message
                width: parent.width
                topPadding: 4
                leftPadding: 20
                rightPadding: 20
                bottomPadding: 4
            }
        }
    }
    Rectangle { // 内容
        id:layoutContent
        width: parent.width
        height: layoutColumn.childrenRect.height
        color: 'transparent'
        radius:5
        ColumnLayout{
            id: layoutColumn
            width: parent.width
            QuickText { // 标题
                id: textTitle
                font: QuickFont.Title
                text:title
                topPadding: 20
                leftPadding: 20
                rightPadding: 20
                wrapMode: Text.WrapAnywhere
            }
            QuickLoader { // 加载消息组件
                sourceComponent: comMessage
                Layout.fillWidth: true
                Layout.preferredHeight: status===Loader.Ready ? item.height : 0
            }
            QuickLoader { // 加载内容
                sourceComponent: control.visible ? control.contentDelegate : undefined
                Layout.fillWidth: true
                onStatusChanged: {
                    if(status===Loader.Ready){
                        Layout.preferredHeight = item.implicitHeight
                    }else{
                        Layout.preferredHeight = 0
                    }
                }
            }
            Rectangle { // 操作按钮布局
                id:layoutActions
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                radius: 5
                color: Qt.rgba(243/255,243/255,243/255,1)
                RowLayout {
                    anchors
                    {
                        centerIn: parent
                        margins: spacing
                        fill: parent
                    }
                    spacing: 10
                    Item{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        QuickButton { // 关闭按钮
                            id:neutralBtn
                            visible: control.buttonFlags&QuickDialogButtonFlag.NeutralButton
                            text: neutralText
                            width: parent.width
                            anchors.centerIn: parent
                            onClicked: {
                                if(control.onNeutralClickListener){
                                    control.onNeutralClickListener()
                                }else{
                                    neutralClicked()
                                    control.close()
                                }
                            }
                        }
                    }
                    Item{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        QuickButton { // 取消按钮
                            id:negativeBtn
                            visible: control.buttonFlags&QuickDialogButtonFlag.NegativeButton
                            width: parent.width
                            anchors.centerIn: parent
                            text: negativeText
                            onClicked: {
                                if(control.onNegativeClickListener){
                                    control.onNegativeClickListener()
                                }else{
                                    negativeClicked()
                                    control.close()
                                }
                            }
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        QuickFilledButton { // 确认按钮
                            id:positive_btn
                            visible: control.buttonFlags&QuickDialogButtonFlag.PositiveButton
                            text: positiveText
                            width: parent.width
                            anchors.centerIn: parent
                            onClicked: {
                                if(control.onPositiveClickListener){
                                    control.onPositiveClickListener()
                                }else{
                                    positiveClicked()
                                    control.close()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
