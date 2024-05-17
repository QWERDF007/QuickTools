import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui
import QuickTools.core

Rectangle {
    id: lsidebar

    property alias backgroundColor: lsidebar.border.color
    property bool childrenEnable: true
    property alias helpInfos: ltoolview.helpInfos
    property alias inputParams: ltoolview.inputParams
    property alias outputParams: ltoolview.outputParams

    border.color: QuickColor.WindowBackground
    border.width: 1
    height: parent.height
    width: 64 + 1 + 256

    RowLayout {
        anchors.fill: parent
        spacing: 0

        LToolBar {
            id: ltoolbar

            Layout.fillHeight: true
            width: 64
        }
        Rectangle {
            Layout.fillHeight: true
            color: lsidebar.backgroundColor // 分割线
            width: 1
        }
        LToolView {
            id: ltoolview

            Layout.fillHeight: true
            Layout.fillWidth: true
            currentIndex: ltoolbar.currentIndex
            itemEnable: lsidebar.childrenEnable
        }
    }
}
