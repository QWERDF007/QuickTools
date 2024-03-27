import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui


Rectangle {
    id: paramItemDelegate
    width: 200
    border.width: 1
    border.color: QuickColor.WindowBackground
    height: 48
    property string paramName: ""
    property var paramValue
    property int paramType: -1
    property string paramTypeName: ""

    Column {
        anchors.fill: paramItemDelegate
        anchors.topMargin: 2
        anchors.bottomMargin: 2

        T_ParamItemDelegateHeader {
            width: parent.width
            height: 20
            paramName: paramItemDelegate.paramName
            paramTypeName: paramItemDelegate.paramTypeName
        }

        Rectangle {
            width: parent.width
            height: 1
            color: paramItemDelegate.border.color
        }

        T_ParamItemDelegateContent {
            width: parent.width
            height: 24
            paramType: paramItemDelegate.paramType
            paramValue: paramItemDelegate.paramValue
        }

    }
}
