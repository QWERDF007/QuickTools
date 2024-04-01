import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core
import "./paramitem"


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
    signal paramChanged(var value)

    Component {
        id: statusParamItem
        StatusParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: intParamItem
        IntParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: doubleParamItem
        DoubleParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: textParamItem
        TextParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: int1DArrayParamItem
        Int1DArrayParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: double1DArrayParamItem
        Double1DArrayParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: text1DArrayParamItem
        Text1DArrayParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: int2DArrayParamItem
        Int2DArrayParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: double2DArrayParamItem
        Double2DArrayParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: text2DArrayParamItem
        Text2DArrayParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Component {
        id: inputImageParamItem
        InputImageParamItem {
            paramIndex: paramItemDelegate.paramIndex
            paramName: paramItemDelegate.paramName
            paramValue: paramItemDelegate.paramValue
            onValueChanged: function(value) { paramItemDelegate.paramChanged(value) }
        }
    }

    Column {
        anchors.fill: parent
        anchors.topMargin: 2
        anchors.bottomMargin: 2


        Item { // 参数头, 显示参数名称和鼠标悬浮显示参数类型信息
            id: paramHeader
            width: parent.width
            height: 20
            QuickText {
                anchors.fill: parent
                text: paramItemDelegate.paramDisplayName
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            ToolTip {
                visible: paramItemDelegate.paramTypeName !== "" && headerMouseArea.containsMouse
                delay: 200
                text: "参数类型: " + paramTypeName
            }
            MouseArea {
                id: headerMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }
        }

        Rectangle {
            width: parent.width
            height: 1
            color: paramItemDelegate.border.color
        }

        QuickLoader {
            id: loader
            width: parent.width
            height: 24
            sourceComponent: {
                if (paramItemDelegate.paramType === QuickToolParam.ParamStatusType) {
                    return statusParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamIntType) {
                    return intParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamDoubleType) {
                    return doubleParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamTextType) {
                    return textParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamInt1DArrayType) {
                    return int1DArrayParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamDouble1DArrayType) {
                    return double1DArrayParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamText1DArrayType) {
                    return text1DArrayParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamInt2DArrayType) {
                    return int2DArrayParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamDouble2DArrayType) {
                    return double2DArrayParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamText2DArrayType) {
                    return text2DArrayParamItem
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamImageType) {
                    return inputImageParamItem
                }
                return undefined
            }
        }

    }
    function forwardValueChanged(value) {
        paramChanged(value)
    }
}
