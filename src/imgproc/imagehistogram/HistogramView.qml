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
    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            implicitWidth: childrenRect.width
            height: 24
            border.width: 1
            border.color: QuickColor.WindowBackground
            color: QuickColor.White
            ListView {
                id: tabbar
                orientation: ListView.Horizontal
                implicitWidth: childrenRect.width
                height: 24
                boundsBehavior: Flickable.StopAtBounds
                model: histogramsData === undefined || histogramsData === null ? 0 : histogramsData.length
                delegate: Rectangle {
                    width: 24
                    height: 24
                    color: {
                        if (mouseArea.pressed) {
                            return QuickColor.ItemPress
                        }  else if (mouseArea.hovered) {
                            return QuickColor.ItemHover
                        } else if (tabbar.currentIndex === model.index) {
                            return QuickColor.ItemCheck
                        }
                        return QuickColor.ItemNormal
                    }

                    QuickText {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: index
                    }
                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        onClicked: {
                            tabbar.currentIndex = index
                        }
                    }
                }
            }
        }

        StackLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            currentIndex: tabbar.currentIndex
            Repeater {
                model: histogramsData === undefined || histogramsData === null ? 0 : histogramsData.length
                ChartView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    legend.visible: false
                    antialiasing: true

                    ToolTip {
                        id: tooltip
                        delay: 200
                        visible: false
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
}
