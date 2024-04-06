import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import Qt.labs.platform

import QuickTools.core
import QuickTools.ui


T_CVWindow {
    id: imageHistogramWin
    width: 1080
    height: 720
    visible: true

    quicktool: QuickToolFactor.createQuickTool(QuickToolType.ImageHistogram, imageHistogramWin)
    inputParams: quicktool.inputParams
    outputParams: quicktool.outputParams
    activateItem: image

    Component.onCompleted: {
        console.log("quick tool", quicktool.name)
        console.log("input params", inputParams.name)
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

            // Image {
            //     id: image
            //     anchors.fill: parent
            //     fillMode: Image.PreserveAspectFit
            //     source: {
            //         if (inputParams.pdata.Image === null || inputParams.pdata.Image === undefined) {
            //             return ""
            //         }
            //         return "file:///" + inputParams.pdata.Image
            //     }
            // }

            QuickScaleableImage {
                id: image
                anchors.fill: parent
                image.source: {
                    if (inputParams.pdata.Image === null || inputParams.pdata.Image === undefined) {
                        return ""
                    }
                    return "file:///" + inputParams.pdata.Image
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
