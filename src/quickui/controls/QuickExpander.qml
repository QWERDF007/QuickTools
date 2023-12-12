import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
//import QtQuick.Window

Item {
    property alias headerText: headerLabel.text
//    property alias headerFont: headerLabel.font
    property alias headerPixelSize: headerLabel.font.pixelSize
    property alias headerHeight: layout_header.height
    property bool expand: true
    property bool init: true // 优化初始化折叠显示
    property int contentHeight : height - layout_header.height
    property int duration: 500
    default property alias content: container.data
    id:control
    implicitHeight: Math.max((layout_header.height + container.height),layout_header.height)
    implicitWidth: 400
    Item {
        id:layout_header
        width: parent.width
        height: 32
        RowLayout {
            width: parent.width
            ToolButton {
                id: toolBtn
                icon.source: "/icons/expand"
                icon.height: 10
                icon.width: 8
                Layout.alignment: Qt.AlignLeft
                rotation: expand ? 180 : 0
                implicitHeight: 28
                implicitWidth: 28
                onClicked: {
                    expand = !expand
                    init = false
                }
                Behavior on rotation {
                    NumberAnimation{ duration: control.duration; easing.type: Easing.OutCubic }
                }
            }
            Label {
                id: headerLabel
                text: qsTr("Title")
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                font.pixelSize: 16
            }
        }
    }
    Item{
        anchors{
            top: layout_header.bottom
            topMargin: -1
            left: layout_header.left
        }
        width: parent.width
        clip: true
        visible: control.init && !control.expand ? false :  contentHeight+container.y !== 0
        height: contentHeight+container.y
        Item {
            id:container
            width: parent.width
            height: parent.height
            y: -contentHeight
            states: [
                State{
                    name:"expand"
                    when: control.expand
                    PropertyChanges { target: container; y:0 }
                },
                State{
                    name:"collapsed"
                    when: !control.expand
                    PropertyChanges { target: container; y:-contentHeight }
                }
            ]
            transitions: [
                Transition {
                    to:"expand"
                    NumberAnimation { properties: "y"; duration: control.duration; easing.type: Easing.OutCubic }
                },
                Transition {
                    to:"collapsed"
                    NumberAnimation { properties: "y"; duration: control.duration; easing.type: Easing.OutCubic }
                }
            ]
        }
    }
}
