import QtQuick
import QtQuick.Controls
import Qt.labs.qmlmodels
import QuickTools.ui
import QuickTools.core
import quickui

QuiPage {
    HorizontalHeaderView {
        id: horizontalHeader
        property int minimalWidth: 48
        anchors.left: tableView.left
        anchors.top: parent.top
        syncView: tableView
        clip: true

        columnWidthProvider: function (column) {
            let w = Math.max(explicitColumnWidth(column), tableView.explicitColumnWidth(column))
            if (w >= 0)
                return w > minimalWidth ? w : minimalWidth
            w = Math.max(implicitColumnWidth(column), tableView.implicitColumnWidth(column))
            return w > minimalWidth ? w : minimalWidth
        }
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
        columnWidthProvider: function (column) {
            let w = explicitColumnWidth(column)
            if (w >= 0)
                return w
            return implicitColumnWidth(column)
        }
        delegate: DelegateChooser {
            DelegateChoice {
                column: 2
                delegate: Row {
                    property bool running: display
                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        width: 10
                        height: 10
                        radius: width / 2
                        color: running ? "red" : "green"
                    }
                    QuiText {
                        text: running ? qsTr("运行中") : qsTr("空闲")
                    }
                }
            }
            DelegateChoice {
                delegate: QuiText {
                    text: display
                }
            }

        }
    }
}
