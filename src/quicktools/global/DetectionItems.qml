pragma Singleton

import QtQuick
import QtQuick.Controls

import QuickTools.core
import "../component"

ToolsList {
    id: list
    group: QuickToolManager.getGroupUUID(QuickToolGroupType.SamplesGroup)
    ToolItem {
        title: "Yolov8 Detection"
        desc: qsTr("Yolov8 目标检测工具")
        icon: "/icons/bar"
        url: "qrc:/qt/qml/QuickTools/deeplearning/detection/yolov8/Yolov8.qml"
        extra: ({recentlyUpdated:true,recentlyAdded:true})
        group: list.group
        task: QuickToolManager.getTaskUUID(QuickToolTaskType.DetectionTask)
    }
}

