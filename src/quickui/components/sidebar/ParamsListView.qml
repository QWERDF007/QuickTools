import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core

Item {
    id: paramsListView
    width: 200
    height: 400
    property color delegateBackgroundColor: QuickColor.White
    property color delegateBorderColor: QuickColor.WindowBackground

    property ToolParams toolParams

    ListView {
        id: _paramsListView
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        boundsBehavior: Flickable.StopAtBounds
        spacing: 10
        model: toolParams
        delegate: ParamItemDelegate {
            width: _paramsListView.width
            height: 48
            color: delegateBackgroundColor
            border.width: 1
            border.color: delegateBorderColor
            toolParams: paramsListView.toolParams
            paramIndex: model.paramIndex
            paramName: model.paramName
            paramType: model.paramType
            paramTypeName: model.paramTypeName
            paramValue: model.paramValue
            paramDisplayName: model.paramDisplayName
            Component.onCompleted: {
                console.log(model.paramIndex, model.paramName, model.paramDisplayName, model.paramType, model.paramTypeName, model.paramValue)
            }
        }
    }
}
