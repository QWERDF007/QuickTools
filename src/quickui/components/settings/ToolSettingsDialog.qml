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
    }
}
