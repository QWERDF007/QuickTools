import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

Window {
    property alias settingsModel: settingsView.model

    title: qsTr("Settings")

    ListView {
        id: settingsView

        anchors.bottomMargin: 5
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.topMargin: 5
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        spacing: 5

        ScrollBar.vertical: QuickScrollBar {
        }
        delegate: SettingItemDelegate {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 5
            height: 32
        }

        section {
            criteria: ViewSection.FullString
            property: "groupName"

            delegate: Item {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 5
                height: 48

                QuickText {
                    font: QuickFont.Title
                    text: section
                }
            }
        }
    }
}
