import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui


Rectangle {
    id: lsidebar
    border.width: 1
    border.color: QuickColor.WindowBackground
    width: 64 + 1 + 256
    height: parent.height

    property alias backgroundColor: lsidebar.border.color
    property alias inputParamsModel: ltoolview.inputParamsModel
    property alias outputParamsModel: ltoolview.outputParamsModel

    RowLayout {
        anchors.fill: parent
        T_CVLToolBar {
            id: ltoolbar
            width: 64
            Layout.fillHeight: true
        }
        Rectangle {
            width: 1
            Layout.fillHeight: true
            color: lsidebar.backgroundColor
        }
        T_CVLToolView {
            id: ltoolview
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: ltoolbar.currentIndex
        }
    }
}
