import QtQuick
import QtQuick.Controls

import QuickTools.ui
import QuickTools.core

// 用于定义 T_Page 中的项目
QtObject {
    id:control
    property var url
    property string desc
    property string title
    property string icon
    property var extra
    property string task: QuickToolManager.getTaskUUID(QuickToolTaskType.EmptyTask)
    property string group: QuickToolManager.getGroupUUID(QuickToolGroupType.EmptyGroup)
}
