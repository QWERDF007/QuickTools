import QtQuick
import QtQuick.Controls

import QuickTools.ui
import QuickTools.core

QuickObject {
    property string group: QuickToolManager.getGroupUUID(QuickToolGroupType.EmptyGroup)
    property int _idx
    property bool visible: true
    property string title
    property var icon
    property bool disabled: false
    property bool iconVisible: true
    property bool isExpand: false
    property bool showEdit
    property Component iconDelegate
    property Component menuDelegate
    property Component editDelegate
}
