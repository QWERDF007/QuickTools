import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core

import "./paramitem"

Item {
    id: paramContent
    width: 200
    height: 24
    property int paramIndex: -1
    property int paramType: -1
    property var paramValue
    property string paramName: ""
    property ToolParams toolParams

    Component {
        id: statusItem
        StatusParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: intItem
        IntParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: doubleItem
        DoubleParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: textItem
        TextParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: int1DArrayItem
        Int1DArrayParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: double1DArrayItem
        Double1DArrayParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: text1DArrayItem
        Text1DArrayParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: int2DArrayItem
        Int2DArrayParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: double2DArrayItem
        Double2DArrayParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: text2DArrayItem
        Text2DArrayParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
        }
    }

    Component {
        id: inputImageItem
        InputImageParamItem {
            paramIndex: paramContent.paramIndex
            paramName: paramContent.paramName
            paramValue: paramContent.paramValue
            toolParams: paramContent.toolParams
            Component.onCompleted: {
                console.log("ParamItemContent toolParams", toolParams)
            }
            onToolParamsChanged: {
                console.log("ParamItemContent onToolParamsChanged toolParams", toolParams)
            }
        }
    }

    QuickLoader {
        anchors.fill: parent
        sourceComponent: {
            if (paramContent.paramType === QuickToolParam.ParamStatusType) {
                return statusItem
            } else if (paramContent.paramType === QuickToolParam.ParamIntType) {
                return intItem
            } else if (paramContent.paramType === QuickToolParam.ParamDoubleType) {
                return doubleItem
            } else if (paramContent.paramType === QuickToolParam.ParamTextType) {
                return textItem
            } else if (paramContent.paramType === QuickToolParam.ParamInt1DArrayType) {
                return int1DArrayItem
            } else if (paramContent.paramType === QuickToolParam.ParamDouble1DArrayType) {
                return double1DArrayItem
            } else if (paramContent.paramType === QuickToolParam.ParamText1DArrayType) {
                return text1DArrayItem
            } else if (paramContent.paramType === QuickToolParam.ParamInt2DArrayType) {
                return int2DArrayItem
            } else if (paramContent.paramType === QuickToolParam.ParamDouble2DArrayType) {
                return double2DArrayItem
            } else if (paramContent.paramType === QuickToolParam.ParamText2DArrayType) {
                return text2DArrayItem
            } else if (paramContent.paramType === QuickToolParam.ParamImageType) {
                return inputImageItem
            }
            return undefined
        }
    }
}
