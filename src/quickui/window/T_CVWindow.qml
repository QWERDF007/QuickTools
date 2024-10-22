import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui
import QuickTools.core
import "../components/header"
import "../components/footer"

T_Window {
    id: window

    property var activateItem
    property color drawingColor: toolSettings ? UITools.withOpacity(toolSettings.pdata.ROIColor, toolSettings.pdata.ROIColorAlpha) :
                                                UITools.withOpacity("red", 0.5)
    property color drawingBorderColor: toolSettings ? toolSettings.pdata.ROIBorderColor : "red"

    property CVToolROI inputROI: inputParams ? inputParams.roi : null
    property int acceptedShapes: quicktool && quicktool.hasOwnProperty("acceptedShapes") ? quicktool.acceptedShapes : QuickShape.NoShape


    signal fitInWindow
    signal sliderMoved(real value)

    height: 720
    width: 1440


    header: CVHeader {
        id: _header

        height: 40
        width: window.width
        hasPython: window.hasPython
        activateItem: window.activateItem
        acceptedShapes: window.acceptedShapes
        enabled: window.enabled
        onSettingsBtnClicked: window.openSettings()
        onStartBtnClicked: window.run()
        onReloadBtnClicked: window.reloadModule()
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
        inputROI.shapeType = shapeType
        inputROI.data = data
    }
}
