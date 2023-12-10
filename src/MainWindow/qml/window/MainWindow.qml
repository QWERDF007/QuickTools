import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true
    width: 1920
    height: 1080
//    color: "red"

    menuBar: MenuBar {
        implicitHeight: 12
    }

    header: ToolBar {
        implicitHeight: 24
    }

    footer: TabBar {

    }

    Item {

        anchors.fill: parent
        RowLayout {
            anchors.fill: parent
            ListView {
                Layout.preferredWidth: 240
                Layout.fillHeight: true
            }
            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "gray"
            }
        }
    }
}
