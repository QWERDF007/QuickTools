﻿pragma Singleton

import QtQuick
import QtQuick.Controls

import "../component"

ToolsList {
    ToolItem {
        title: qsTr("图像直方图")
        desc: qsTr("图像中像素值分布, 横坐标为像素值, 纵坐标为像素值的统计")
        icon: "/icons/bar"
        url: "qrc:/qt/qml/QuickTools/imgproc/imagehistogram/ImageHistogram.qml"
    }
}
