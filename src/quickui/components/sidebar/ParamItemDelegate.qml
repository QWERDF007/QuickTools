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
    property ToolParams toolParams
    signal valueChanged(var value)

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
                var itemUrl = ""
                if (paramItemDelegate.paramType === QuickToolParam.ParamStatusType) {
                    itemUrl = "./paramitem/StatusParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamIntType) {
                    itemUrl = "./paramitem/IntParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamDoubleType) {
                    itemUrl = "./paramitem/DoubleParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamTextType) {
                    itemUrl = "./paramitem/TextParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamInt1DArrayType) {
                    itemUrl = "./paramitem/Int1DArrayParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamDouble1DArrayType) {
                    itemUrl = "./paramitem/Double1DArrayParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamText1DArrayType) {
                    itemUrl = "./paramitem/Text1DArrayParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamInt2DArrayType) {
                    itemUrl = "./paramitem/Int2DArrayParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamDouble2DArrayType) {
                    itemUrl = "./paramitem/Double2DArrayParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamText2DArrayType) {
                    itemUrl = "./paramitem/Text2DArrayParamItem.qml"
                } else if (paramItemDelegate.paramType === QuickToolParam.ParamImageType) {
                    itemUrl = "./paramitem/InputImageParamItem.qml"
                }
                if (itemUrl === "") {
                    return undefined
                }
                var component = Qt.createComponent(itemUrl)
                if (component.status === Component.Ready) {
                    var imageItem = component.createObject(loader, { paramIndex: paramItemDelegate.paramIndex, paramName: paramItemDelegate.paramName, paramValue: paramItemDelegate.paramValue, toolParams: paramItemDelegate.toolParams })
                    if (imageItem === null)  {
                         console.log("Error creating object")
                    } else {
                        imageItem.valueChanged.connect(paramItemDelegate.valueChanged)
                    }
                } else if (component.status === Component.Error) {
                    console.log("Error loading component:", component.errorString())
                }
                return undefined
            }
        }
    }
}
