import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import Qt.labs.platform

import QuickTools.ui

Item {
    id: histogramContainer
    width: 200
    height: 40
    property var histogramsData
    property var histogramsMin
    property var histogramsMax

    QuickSplitView {
        anchors.fill: parent
        orientation: Qt.Vertical
        Repeater {
            model: {
                if (histogramsData === undefined || histogramsData === null) {
                    return 0
                } else if (histogramsData.length >= 3) {
                    return 3
                } else {
                    return histogramsData.length
                }
            }

            ChartView {
                id: chartview
                SplitView.fillWidth: true
                SplitView.minimumHeight: 200
                implicitHeight: parent.height / 3
                legend.visible: false
                antialiasing: true

                ToolTip {
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
                    barWidth: 1
                    //                        labelsAngle: 270
                    //                        labelsVisible: true
                    //                        labelsPosition: AbstractBarSeries.LabelsInsideEnd
                    axisX: ValuesAxis {
                        tickCount: 11
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
}
