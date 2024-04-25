import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import Qt.labs.platform

import QuickTools.ui

Item {
    id: histogram
    width: 200
    height: 40
    property var histogramsData
    property var histogramsMin
    property var histogramsMax
    property int tickCount: 11
    property real barWidth: 1
    property real chartHeight: 400


    ListView {
        id: lv
        anchors.fill: parent
        ScrollBar.vertical: QuickScrollBar{}
        boundsBehavior: Flickable.StopAtBounds
        model: {
            if (histogramsData === undefined || histogramsData === null) {
                return 0
            } else if (histogramsData.length >= 3) {
                return 3
            } else {
                return histogramsData.length
            }
        }

        delegate:  ChartView {
            id: chartview
            width: lv.width - 8
            height: histogram.chartHeight
            legend.visible: false
            antialiasing: true

            QuickToolTip {
                id: tooltip
                delay: 200
                visible: false
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            BarSeries {
                barWidth: histogram.barWidth
                //                        labelsAngle: 270
                //                        labelsVisible: true
                //                        labelsPosition: AbstractBarSeries.LabelsInsideEnd
                axisX: ValuesAxis {
                    tickCount: histogram.tickCount
                    min: 0
                    max: histogramsData === null || histogramsData === undefined ? 255 : histogramsData[index].length
                }
                onHovered: function(status, i, barset) {
                    if (mouseArea.mouseX + tooltip.contentWidth + 20 > chartview.width) {
                        tooltip.x = mouseArea.mouseX - tooltip.contentWidth - 20
                    } else {
                        tooltip.x = mouseArea.mouseX
                    }
                    if (mouseArea.mouseY + tooltip.contentHeight + 20 > chartview.height) {
                        tooltip.y = mouseArea.mouseY - tooltip.contentHeight - 20
                    } else {
                        tooltip.y = mouseArea.mouseY + 20
                    }
                    tooltip.visible = status
                    tooltip.text = "x: " + i + " y: " + barset.values[i]
                }

                axisY: ValuesAxis {
                    tickCount: 11
                    min: 0
                    max: histogramsMax === null || histogramsMax === undefined || histogramsMax[index] === undefined ? 1 : histogramsMax[index]
                }
                BarSet {
                    values: histogramsData === null || histogramsData === undefined ? [0] : histogramsData[index]
                }
            }
        }
    }
}
