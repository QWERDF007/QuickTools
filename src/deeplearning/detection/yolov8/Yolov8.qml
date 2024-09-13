import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import Qt.labs.platform
import QuickTools.core
import QuickTools.ui

T_CVWindow {
    id: yolov8Win

    acceptedShapes: QuickShapeType.Rectangle
    activateItem: image
    quicktool: QuickToolManager.createQuickTool(QuickToolType.Yolov8DetectionTool, yolov8Win)

    QuickSplitView {
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
                image.source: {
                    if (inputParams.pdata.Image) {
                        return "file:///" + inputParams.pdata.Image
                    }
                    return ""
                }

                onRoiDataChanged: function (shapeType, data) {
                    updateROI(shapeType, data)
                }

                shapes: outputParams.pdata.Rects
            }
            //        DropImageArea {
            //            anchors.fill: parent
            //            dropBtnAreaVisible: image.status === Image.Null

            //            onPathChanged: function (path) {
            //                inputParams.pdata.Image = path
            //            }
            //        }
        }
//        Item {
//            id: outputImageContainer

//            SplitView.fillHeight: true
//            SplitView.fillWidth: true
//            SplitView.minimumWidth: 160
//            implicitWidth: sv1.width / 2
//            clip: true
//            visible: outputParams.pdata.Status && outputParams.pdata.Status[0] === 0 ? true : false

//            QuickScalableImage {

//                anchors.fill: parent
//                drawingColor: yolov8Win.drawingColor
//                drawingBorderColor: yolov8Win.drawingBorderColor
//                image.source: {
//                    if (inputParams.pdata.Image) {
//                        return "file:///" + inputParams.pdata.Image
//                    }
//                    return ""
//                }

//                onRoiDataChanged: function (shapeType, data) {
//                    updateROI(shapeType, data)
//                }
//            }
//        }
    }
}
