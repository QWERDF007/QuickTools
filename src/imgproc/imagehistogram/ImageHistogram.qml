import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts

import QuickTools.core
import QuickTools.ui

T_CVWindow {
    id: imageHistogramWin
    width: 1080
    height: 720
    visible: true

    property QuickTool quicktool: QuickToolFactor.createQuickTool(QuickToolType.ImageHistogram)
    inputParams: quicktool.inputParams
    outputParams: quicktool.outputParams

    Component.onCompleted: {
        console.log("quick tool", quicktool.name)
        console.log("input params", inputParams.name)
        console.log("output params", outputParams.name)
        console.log("Image", inputParams.pdata.Image)
        console.log("Hist", outputParams.pdata.Hist)
    }

    Connections {
        target: quicktool
        function onFinished() {
            console.log("finished!!!")
            console.log("Image", inputParams.pdata.Image)
            console.log("Hist", outputParams.pdata.Hist.length)
            console.log("Hist", outputParams.pdata.Hist[0].length)
        }
    }

    /**
     * @brief 获取 obj 的属性并转换为字符串, 包括 function 和 property
     * @param obj
     */
    function objToString (obj) {
        var str = '';
        for (var p in obj) {
            if (Object.prototype.hasOwnProperty.call(obj, p)) {
                str += p + '::' + obj[p] + '\n';
            }
        }
        return str;
    }

    QuickSplitView {
        id: sv1
        anchors.fill: parent

        QuickArea {
            id: imageContainer
            SplitView.fillHeight: true
            SplitView.fillWidth: true
            SplitView.minimumWidth: 160
            clip: true

            Image {
                id: image
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: {
                    if (inputParams.pdata.Image === null || inputParams.pdata.Image === undefined) {
                        return ""
                    }
                    return "file:///" + inputParams.pdata.Image
                }
            }

            Item {
                id: dropBtnArea
                visible: image.status === Image.Null
                implicitHeight: childrenRect.height
                anchors.centerIn: parent
                Image {
                    id: dropimage
                    width: 80
                    height: 80
                    anchors {
                        top: parent.top
                        horizontalCenter: parent.horizontalCenter
                    }
                    source: "/icons/dropimage"
                }
                QuickText {
                    id: droptip
                    anchors {
                        top: dropimage.bottom
                        topMargin: 5
                        horizontalCenter: parent.horizontalCenter
                    }

                    text: "DROP YOUR IMAGES HERE"
                }
                QuickText {
                    id: seperator
                    anchors {
                        top: droptip.bottom
                        topMargin: 5
                        horizontalCenter: parent.horizontalCenter
                    }
                    text: "- OR -"
                }

                QuickButton {
                    id: browseBtn
                    anchors {
                        top: seperator.bottom
                        topMargin: 5
                        horizontalCenter: parent.horizontalCenter
                    }
                    text: "BROWSE FROM YOUR COMPUTER"
                }
            }

            DropArea {
                anchors.fill: parent
                onDropped: function(drop) {
                    console.log("drop", drop.urls)
                    var url = drop.urls[0]
                    if (isImageFile(url) && image.source !== url) {
                        var path = getImagePath(url)
                        inputParams.pdata.Image = path
                    }
                }

                function getImagePath(url) {
                    var path = url.toString().toLowerCase()
                    return path.slice(8)
                }

                function isImageFile(url) {
                    var path = url.toString().toLowerCase()
                    return path.startsWith("file:") && (path.endsWith(".jpg") || path.endsWith(".png") || path.endWith(".jpeg"))
                }
            }
        }
        Item {
            id: histogramContainer
            visible: {
                if (outputParams.pdata.Hist === undefined || outputParams.pdata.Hist === null) {
                    return false
                } else {
                    return true
                }
            }

            implicitWidth: sv1.width / 2

            ChartView {
                title: "Bar series"
                anchors.fill: parent
                legend.visible: false
                antialiasing: true

                BarSeries {
                    id: mySeries
                    barWidth: 1
                    axisX: ValuesAxis {
                        tickCount: 11
                        min: 0
                        max: {
                            if (outputParams.pdata.Hist === undefined || outputParams.pdata.Hist === null) {
                                return 255
                            } else {
                                return outputParams.pdata.Hist[0].length
                            }
                        }
                    }
                    axisY: ValuesAxis {
                        tickCount: 11
                        min: 0
                        max: 1
                    }
                    BarSet {
                        values: {
                            if (outputParams.pdata.Hist === undefined || outputParams.pdata.Hist === null) {
                                return [0]
                            } else {
                                return outputParams.pdata.Hist[0]
                            }
                        }
                    }
                }
            }
        }
    }
}
