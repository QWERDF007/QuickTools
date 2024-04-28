import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import Qt.labs.platform

import QuickTools.core
import QuickTools.ui

T_CVWindow {
    id: imageHistogramWin

    quicktool: QuickToolFactor.createQuickTool(QuickToolType.ImageHistogram, imageHistogramWin)
    inputParams: quicktool.inputParams
    inputROI: inputParams.roi
    outputParams: quicktool.outputParams
    helpInfos: quicktool.doc
    activateItem: image
    polygonEnable: false

    Component.onCompleted: {
        console.log("quick tool", quicktool.name)
        console.log("input params", inputParams.name)
        console.log("input roi", inputROI)
        console.log("output params", outputParams.name)
        console.log("Image", inputParams.pdata.Image)
        console.log("Hist", outputParams.pdata.Hist)
    }

    QuickSplitView {
        id: sv1
        anchors.fill: parent

        Item {
            id: imageContainer
            SplitView.fillHeight: true
            SplitView.fillWidth: true
            SplitView.minimumWidth: 160
            clip: true


            QuickScalableImage {
                id: image
                anchors.fill: parent
                image.source: {
                    if (inputParams.pdata.Image === null || inputParams.pdata.Image === undefined) {
                        return ""
                    }
                    return "file:///" + inputParams.pdata.Image
                }
                drawingColor: imageHistogramWin.drawingColor
                drawingBorderColor: imageHistogramWin.drawingBorderColor
                onRoiDataChanged: function(shapeType, data) {
                    updateROI(shapeType, data)
                }
            }

            DropImageArea {
                anchors.fill: parent
                dropBtnAreaVisible: image.status === Image.Null
                onPathChanged: function(path) {
                    inputParams.pdata.Image = path
                }
            }
        }

        HistogramView {
            visible: outputParams.pdata.Hist === null || outputParams.pdata.Hist === undefined ? false : true
            SplitView.fillHeight: true
            implicitWidth: sv1.width / 2
            histogramsData: outputParams.pdata.Hist
            histogramsMin: outputParams.pdata.HistMin
            histogramsMax: outputParams.pdata.HistMax
        }
    }
}
