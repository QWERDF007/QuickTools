import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

QuickPage {
    default property alias content: container.data
    Flickable{
        clip: true
        anchors.fill: parent
        ScrollBar.vertical: QuickScrollBar {}
        boundsBehavior: Flickable.StopAtBounds
        contentHeight: container.height
        ColumnLayout{
            id:container
            width: parent.width
        }
    }
}
