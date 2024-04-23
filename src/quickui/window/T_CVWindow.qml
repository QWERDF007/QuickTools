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
    property alias squareChecked: _header.squareChecked
    property alias circleChecked: _header.circleChecked
    property alias polygonChecked: _header.polygonChecked

    property CVInputParams inputParams
    property CVOutputParams outputParams
    property QuickToolROI inputROI

    signal sliderMoved(real value)
    signal fitInWindow

    header: T_CVHeader {
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
        }
        Item {
            id: container
            enabled: window.enabled
            SplitView.fillHeight: true
            SplitView.fillWidth: true
        }
    }

    footer: T_CVFooter {
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
            inputROI.roiType = QuickToolROI.NoROI
            inputROI.data = data
        } else if (shapeType === QuickShape.Rectangle) {
            inputROI.roiType = QuickToolROI.Rectangle
            inputROI.data = data
        } else if (shapeType === QuickShape.Circle) {
            inputROI.roiType = QuickToolROI.Circle
            inputROI.data = data
        } else if (shapeType === QuickShape.Polygon) {

        }
    }
}
