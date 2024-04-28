import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core

import "../components/header"
import "../components/sidebar"
import "../components/footer"


T_Window {
    id: window
    width: 1440
    height: 720

    default property alias content: container.data
    property var activateItem

    property alias rectangleEnable: _header.rectangleEnable
    property alias circleEnable: _header.circleEnable
    property alias polygonEnable: _header.polygonEnable
    property alias rectangleChecked: _header.rectangleChecked
    property alias circleChecked: _header.circleChecked
    property alias polygonChecked: _header.polygonChecked
    property color drawingColor: UITools.withOpacity("lightblue", 0.5)
    property color drawingBorderColor: "red"

    property CVInputParams inputParams
    property CVOutputParams outputParams
    property CVToolROI inputROI

    signal sliderMoved(real value)
    signal fitInWindow

    header: CVHeader {
        id: _header
        enabled: window.enabled
        width: parent.width
        height: 40
        activateItem: window.activateItem
        onStartBtnClicked: run()
    }



    QuickSplitView {
        id: splitView
        anchors.fill: parent
        LSideBar {
            id: lsidebar
            childrenEnable: window.enabled
            border.color: window.color
            SplitView.minimumWidth: 256
            SplitView.preferredWidth: 321
            SplitView.fillHeight: true
            inputParams: window.inputParams
            outputParams: window.outputParams
            helpInfos: window.helpInfos
        }
        Item {
            id: container
            enabled: window.enabled
            SplitView.fillHeight: true
            SplitView.fillWidth: true
        }
    }

    footer: CVFooter {
        id: _footer
        enabled: window.enabled
        width: parent.width
        height: 40
        activateItem: window.activateItem
        onSliderMoved: function(value) {
            window.sliderMoved(value)
        }
        onFitInWindow: window.fitInWindow()
    }

    function updateROI(shapeType, data) {
        if (inputROI === null) {
            return
        }
        if (shapeType === QuickShape.NoShape) {
            inputROI.roiType = CVToolROI.NoROI
            inputROI.data = data
        } else if (shapeType === QuickShape.Rectangle) {
            inputROI.roiType = CVToolROI.Rectangle
            inputROI.data = data
        } else if (shapeType === QuickShape.Circle) {
            inputROI.roiType = CVToolROI.Circle
            inputROI.data = data
        } else if (shapeType === QuickShape.Polygon) {

        }
    }
}
