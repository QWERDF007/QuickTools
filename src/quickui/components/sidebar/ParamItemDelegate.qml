import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui
import QuickTools.core
import "./paramitem"

Rectangle {
    id: paramItemDelegate


    property string paramName: ""
    property string paramDisplayName: ""
    property string paramDesc: ""
    property var paramDisplay
    property var paramAdditional
    property int paramType: -1
    property string paramTypeName: ""
    property var paramValue

    signal valueChanged(var value)

    function forwardValueChanged(value) {
        paramChanged(value)
    }

    height: 48
    width: 200

    Component {
        id: statusParamItem

        StatusParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: intParamItem

        IntParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: doubleParamItem

        DoubleParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: textParamItem

        TextParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: int1DArrayParamItem

        Int1DArrayParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: double1DArrayParamItem

        Double1DArrayParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: text1DArrayParamItem

        Text1DArrayParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: int2DArrayParamItem

        Int2DArrayParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: double2DArrayParamItem

        Double2DArrayParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: text2DArrayParamItem

        Text2DArrayParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: inputImageParamItem

        InputImageParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: comboBoxParamItem

        ComboBoxParamItem {
            paramDisplay: paramItemDelegate.paramDisplay
            paramName: paramItemDelegate.paramName
            paramAdditional: paramItemDelegate.paramAdditional
            paramValue: paramItemDelegate.paramValue

            onValueChanged: function (value) {
                paramItemDelegate.valueChanged(value)
            }
        }
    }
    Column {
        anchors.bottomMargin: 2
        anchors.fill: parent
        anchors.topMargin: 2

        Item {
            // 参数头, 显示参数名称和鼠标悬浮显示参数类型信息
            id: paramHeader

            height: 20
            width: parent.width

            QuickText {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                text: paramItemDelegate.paramDisplayName
                verticalAlignment: Text.AlignVCenter
            }
            QuickToolTip {
                delay: 200
                text: paramItemDelegate.paramDesc ? paramItemDelegate.paramDesc + qsTr("\n参数类型: ") + paramTypeName :
                                                    qsTr("参数类型: ") + paramTypeName
                visible: paramItemDelegate.paramTypeName !== "" && headerMouseArea.containsMouse
            }
            MouseArea {
                id: headerMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }
        }
        Rectangle {
            color: paramItemDelegate.border.color
            height: 2

            // 分割线
            width: parent.width
        }
        QuickLoader {
            id: loader

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
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamComboBoxType) {
                    return comboBoxParamItem
                }
                return undefined
            }
            width: parent.width
        }
    }
}
