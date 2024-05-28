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

    property alias acceptedShapes: _header.acceptedShapes
    property var activateItem
    default property alias content: container.data
    property color drawingColor: toolSettings ? UITools.withOpacity(toolSettings.pdata.ROIColor, toolSettings.pdata.ROIColorAlpha) : UITools.withOpacity("red", 0.5)
    property color drawingBorderColor: toolSettings ? toolSettings.pdata.ROIBorderColor : "red"
    property CVInputParams inputParams: quicktool ? quicktool.inputParams : null
    property CVToolROI inputROI: inputParams ? inputParams.roi : null
    property CVOutputParams outputParams: quicktool ? quicktool.outputParams : null

    signal fitInWindow
    signal sliderMoved(real value)

    height: 720
    width: 1440


    header: CVHeader {
        id: _header

        activateItem: window.activateItem
        enabled: window.enabled
        height: 40
        width: parent.width

        onSettingsBtnClicked: openSettings()
        onStartBtnClicked: run()
    }

    QuickSplitView {
        id: splitView

        anchors.fill: parent

        LSideBar {
            id: lsidebar

            SplitView.fillHeight: true
            SplitView.minimumWidth: 256
            SplitView.preferredWidth: 321
            border.color: window.color
            childrenEnable: window.enabled
            helpInfos: window.helpInfos
            inputParams: window.inputParams
            outputParams: window.outputParams
        }
        Item {
            id: container

            SplitView.fillHeight: true
            SplitView.fillWidth: true
            enabled: window.enabled
        }
    }

    footer: CVFooter {
        id: _footer

        activateItem: window.activateItem
        enabled: window.enabled
        height: 40
        width: parent.width

        onFitInWindow: window.fitInWindow()
        onSliderMoved: function (value) {
            window.sliderMoved(value)
        }
    }

    function updateROI(shapeType, data) {
        if (!inputROI) {
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
