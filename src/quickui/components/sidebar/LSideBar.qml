import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core


Rectangle {
    id: lsidebar
    border.width: 1
    border.color: QuickColor.WindowBackground
    width: 64 + 1 + 256
    height: parent.height

    property alias backgroundColor: lsidebar.border.color
    property InputParams inputParams
    property OutputParams outputParams

    RowLayout {
        anchors.fill: parent
        spacing: 0
        LToolBar {
            id: ltoolbar
            width: 64
            Layout.fillHeight: true
        }
        Rectangle {
            width: 1
            Layout.fillHeight: true
            color: lsidebar.backgroundColor
        }
        LToolView {
            id: ltoolview
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: ltoolbar.currentIndex
            inputParams: lsidebar.inputParams
            outputParams: lsidebar.outputParams
        }
    }
}
