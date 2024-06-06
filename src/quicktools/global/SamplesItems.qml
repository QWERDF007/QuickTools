pragma Singleton

import QtQuick
import QtQuick.Controls

import "../component"

ToolsList {
    ToolItem {
        title: qsTr("PyTest")
        desc: qsTr("Python 测试工具")
        icon: "/icons/bar"
        url: "qrc:/qt/qml/QuickTools/samples/PyTest/PyTest.qml"
        extra: ({recentlyUpdated:true,recentlyAdded:true})
    }
}

