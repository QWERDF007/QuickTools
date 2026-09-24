import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import Qt.labs.platform
import quickui
import QuickTools.core
import QuickTools.ui

T_CVWindow {
    id: yolov8Win

    acceptedShapes: QuickShape.Rectangle
    activateItem: image
    quicktool: QuickToolManager.createQuickTool(QuickToolType.Yolov8DetectionTool, yolov8Win)

    QuiSplitView {
        id: sv1
        anchors.fill: parent
        Item {
            id: inputImageContainer

            SplitView.fillHeight: true
            SplitView.fillWidth: true
            SplitView.minimumWidth: 160
            clip: true

            QuickScalableImage {
                id: image

                anchors.fill: parent
                drawingColor: yolov8Win.drawingColor
                drawingBorderColor: yolov8Win.drawingBorderColor

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

                shapes: outputParams.pdata.Rects
            }
        }
    }
}
