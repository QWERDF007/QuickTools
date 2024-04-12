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

    property bool childrenEnable: true

    property alias backgroundColor: lsidebar.border.color
    property alias inputParams: ltoolview.inputParams
    property alias outputParams: ltoolview.outputParams

    RowLayout {
        anchors.fill: parent
        spacing: 0
        LToolBar {
            id: ltoolbar
            width: 64
            Layout.fillHeight: true
        }
        Rectangle { // 分割线
            width: 1
            Layout.fillHeight: true
            color: lsidebar.backgroundColor
        }
        LToolView {
            id: ltoolview
            itemEnable: childrenEnable
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: ltoolbar.currentIndex
        }
    }
}
