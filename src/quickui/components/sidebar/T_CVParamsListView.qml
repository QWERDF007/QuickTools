import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Item {
    width: 200
    height: 400
    property alias model: paramsListView.model
    property color delegateBackgroundColor: QuickColor.White
    property color delegateBorderColor: QuickColor.WindowBackground

    ListView {
        id: paramsListView
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        boundsBehavior: Flickable.StopAtBounds
        spacing: 10
        delegate: T_ParamItemDelegate {
            width: paramsListView.width
            color: delegateBackgroundColor
            border.width: 1
            border.color: delegateBorderColor
            paramName: model.paramName
            paramType: model.paramType
            paramTypeName: model.paramTypeName
            paramValue: model.paramValue
            Component.onCompleted: {
                console.log(model.paramName, model.paramType, model.paramTypeName, model.paramValue)
            }
        }
    }
}
