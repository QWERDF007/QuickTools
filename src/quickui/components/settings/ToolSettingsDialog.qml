import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import QuickTools.ui

Window {
    title: qsTr("Settings")
    property alias settingsModel: settingsView.model

    ListView {
        id: settingsView
        clip: true
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.leftMargin: 5
        ScrollBar.vertical: QuickScrollBar{}
        boundsBehavior: Flickable.StopAtBounds
        spacing: 5

        delegate: Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            height: 24
            Text {
                text: model.name
            }
        }

        section {
            property: "groupName"
            criteria: ViewSection.FullString
            delegate: Rectangle {
                color: "#b0dfb0"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                height: 32
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: section
                }
            }
        }
    }
}
