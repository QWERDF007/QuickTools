import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

Item {
    id: paramsListView

    property color delegateBackgroundColor: QuickColor.White
    property color delegateBorderColor: QuickColor.WindowBackground
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

        // anchors.rightMargin: 5
        ScrollBar.vertical: QuickScrollBar {
        }
        delegate: ParamItemDelegate {
            // width: _paramsListView.width
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 5
            border.color: delegateBorderColor
            border.width: 2
            color: delegateBackgroundColor
            enabled: paramsListView.paramItemEnable
            height: 48
            paramDisplay: model.paramDisplay
            paramDisplayName: model.paramDisplayName
            paramName: model.paramName
            paramRange: model.paramRange
            paramType: model.paramType
            paramTypeName: model.paramTypeName
            paramValue: model.paramValue

            onValueChanged: function (value) {
                model.paramValue = value;
            }
        }
    }
}
