import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: control
    property int leftPadding: 10
    property int topPadding: 0
    property int rightPadding: 10
    property int bottomPadding: 10
    property int spacing : 0
    default property alias content: container.data
    Flickable{
        id:flickview
        clip: true
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: container.height
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: QuickScrollBar {
            anchors.right: flickview.right
            anchors.rightMargin: 2
        }
        ColumnLayout{
            id:container
            spacing: control.spacing
            clip: true
            anchors{
                left: parent.left
                right: parent.right
                top: parent.top
                leftMargin: control.leftPadding
                rightMargin: control.rightPadding
            }
            width: parent.width
        }
    }
}
