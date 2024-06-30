pragma Singleton

import QtQuick
import QtQuick.Controls

import QuickTools.core
import "../component"

ToolsList {
    id: list
    group: QuickToolFactor.getGroupUUID(QuickToolGroupType.SamplesGroup)
    ToolItem {
        title: "PyTest"
        desc: qsTr("Python 测试工具")
        icon: "/icons/bar"
        url: "qrc:/qt/qml/QuickTools/samples/PyTest/PyTest.qml"
        extra: ({recentlyUpdated:true,recentlyAdded:true})
        group: list.group
    }
}

