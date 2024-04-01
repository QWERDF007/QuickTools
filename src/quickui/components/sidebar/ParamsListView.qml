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

    property alias model: _paramsListView.model

    ListView {
        id: _paramsListView
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        boundsBehavior: Flickable.StopAtBounds
        spacing: 10
        delegate: ParamItemDelegate {
            width: _paramsListView.width
            height: 48
            color: delegateBackgroundColor
            border.width: 1
            border.color: delegateBorderColor
            paramIndex: model.paramIndex
            paramName: model.paramName
            paramType: model.paramType
            paramTypeName: model.paramTypeName
            paramValue: model.paramValue
            paramDisplayName: model.paramDisplayName
            onParamChanged: function(value) {
                model.paramValue = value
            }
        }
    }
}
