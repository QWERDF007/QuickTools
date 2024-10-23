import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import Qt.labs.platform
import QuickTools.core
import QuickTools.ui

T_CVWindow {
    id: imageHistogramWin

    activateItem: image
    quicktool: QuickToolManager.createQuickTool(QuickToolType.ImageHistogramTool, imageHistogramWin)

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
                drawingColor: imageHistogramWin.drawingColor
                drawingBorderColor: imageHistogramWin.drawingBorderColor

                Connections {
                    target: inputParams.pdata.Image
                    function onImageChanged() {
                        image.source = ""
                        var url = inputParams.pdata.Image.url
                        image.source = url
                    }
                }

                onRoiDataChanged: function (shapeType, data) {
                    updateROI(shapeType, data)
                }
            }
            DropImageArea {
                anchors.fill: parent
                dropBtnAreaVisible: image.status === Image.Null

                onPathChanged: function (path) {
                    // inputParams.pdata.Image = path
                    inputParams.setData("Image", path)
                }
            }
        }
        HistogramView {
            SplitView.fillHeight: true
            histogramsData: outputParams.pdata.Hist
            histogramsMax: outputParams.pdata.HistMax
            histogramsMin: outputParams.pdata.HistMin
            implicitWidth: sv1.width / 2
            chartHeight: toolSettings ? toolSettings.pdata.ChartHeight : 400
            visible: outputParams.pdata.Hist === null || outputParams.pdata.Hist === undefined ? false : true
        }
    }
}
