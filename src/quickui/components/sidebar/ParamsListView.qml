import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

Item {
    id: paramsListView

    property color delegateBackgroundColor: QuiColor.White
    property color delegateBorderColor: QuiColor.Border
    property alias model: _paramsListView.model
    property bool paramItemEnable: true

    clip: true
    height: 400
    width: 200

    ListView {
        id: _paramsListView

        anchors.bottomMargin: 5
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.topMargin: 5
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        spacing: 10

        ScrollBar.vertical: QuiScrollBar {
        }
        delegate: ParamItemDelegate {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 5
            border.color: delegateBorderColor
            border.width: 1
            radius: 4
            color: delegateBackgroundColor
            enabled: paramsListView.paramItemEnable
            height: 48
            paramDisplay: model.paramDisplay
            paramDisplayName: model.paramDisplayName
            paramName: model.paramName
            paramAdditional: model.paramAdditional
            paramType: model.paramType
            paramTypeName: model.paramTypeName
            paramValue: model.paramValue
            paramDesc: model.paramDesc
            paramEditable: model.paramEditable

            onValueChanged: function (value) {
                model.paramValue = value
            }
        }
    }
}
