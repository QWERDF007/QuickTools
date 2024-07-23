import QtQuick
import QtQuick.Controls
import Qt.labs.qmlmodels
import QuickTools.ui
import QuickTools.core

QuickContentPage {
    HorizontalHeaderView {
        id: horizontalHeader
        anchors.left: tableView.left
        anchors.top: parent.top
        syncView: tableView
        clip: true
    }
    VerticalHeaderView {
        id: verticalHeader
        anchors.top: tableView.top
        anchors.left: parent.left
        syncView: tableView
        clip: true
    }

    TableView {
        id: tableView
        anchors.left: verticalHeader.right
        anchors.top: horizontalHeader.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true

        columnSpacing: 1
        rowSpacing: 1
        model: QuickToolManager.activatedTools
        delegate: QuickText {
            text: display
        }
    }
//    ListView {
//        anchors.fill: parent
//        model: QuickToolManager.activatedTools
//        delegate: Rectangle {
//            height: 32
//            width: childrenRect.width
//            Row {
//                QuickText {
//                    text: model.name
//                }
//                QuickText {
//                    text: model.uuid
//                }
//            }
//        }
//    }
}
