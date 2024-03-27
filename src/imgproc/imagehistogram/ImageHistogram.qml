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
    property InputParams inputParam: quicktool.inputParams
    property OutputParams outputParam: quicktool.outputParams

    inputParamsModel: inputParam
    outputParamsModel: outputParam

    Component.onCompleted: {
        console.log("quick tool", quicktool.name)
        console.log("input params", inputParam.name)
        console.log("output params", outputParam.name)
        console.log("Image", inputParam.pdata.Image)
        console.log("Hist", outputParam.pdata.Hist)
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
                        image.source = url
                        var path = getImagePath(url)
                        inputParam.pdata.Image = path
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
                if (outputParam.pdata.Hist === undefined || outputParam.pdata.Hist === null) {
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
                            if (outputParam.pdata.Hist === undefined || outputParam.pdata.Hist === null) {
                                return 255
                            } else {
                                return outputParam.pdata.Hist[0].length
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
                            if (outputParam.pdata.Hist === undefined || outputParam.pdata.Hist === null) {
                                return [0]
                            } else {
                                return outputParam.pdata.Hist[0]
                            }
                        }
                    }
                }
            }
        }
    }
}
