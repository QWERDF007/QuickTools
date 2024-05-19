import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import Qt.labs.platform
import QuickTools.core
import QuickTools.ui

T_CVWindow {
    id: imageHistogramWin

    acceptedShapes: QuickShape.Rectangle | QuickShape.Circle
    activateItem: image
    quicktool: QuickToolFactor.createQuickTool(QuickToolType.ImageHistogram, imageHistogramWin)

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
                drawingBorderColor: imageHistogramWin.drawingColor
                drawingColor: imageHistogramWin.drawingBorderColor
                image.source: {
                    if (inputParams.pdata.Image === null || inputParams.pdata.Image === undefined) {
                        return ""
                    }
                    return "file:///" + inputParams.pdata.Image
                }

                onRoiDataChanged: function (shapeType, data) {
                    updateROI(shapeType, data)
                }
            }
            DropImageArea {
                anchors.fill: parent
                dropBtnAreaVisible: image.status === Image.Null

                onPathChanged: function (path) {
                    inputParams.pdata.Image = path
                }
            }
        }
        HistogramView {
            SplitView.fillHeight: true
            histogramsData: outputParams.pdata.Hist
            histogramsMax: outputParams.pdata.HistMax
            histogramsMin: outputParams.pdata.HistMin
            implicitWidth: sv1.width / 2
            visible: outputParams.pdata.Hist === null || outputParams.pdata.Hist === undefined ? false : true
        }
    }
}
