import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core


Rectangle {
    id: paramItemDelegate
    width: 200
    height: 48
    property int paramIndex: -1
    property string paramName: ""
    property string paramDisplayName: ""
    property var paramValue
    property int paramType: -1
    property string paramTypeName: ""
    property ToolParams toolParams

    Column {
        anchors.fill: parent
        anchors.topMargin: 2
        anchors.bottomMargin: 2

        ParamItemHeader {
            width: parent.width
            height: 20
            paramDisplayName: paramItemDelegate.paramDisplayName
            paramTypeName: paramItemDelegate.paramTypeName
        }

        Rectangle {
            width: parent.width
            height: 1
            color: paramItemDelegate.border.color
        }

        ParamItemContent {
            id: paramItemContent
            width: parent.width
            height: 24
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramType: paramItemDelegate.paramType
            paramValue: paramItemDelegate.paramValue
            toolParams: paramItemDelegate.toolParams
        }
    }
}
