import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

import "../components/header"
import "../components/sidebar"
import "../components/footer"


ApplicationWindow {
    id: window
    width: 1440
    height: 720
    visible: true
    color: active ? QuickColor.WindowActiveBackground : QuickColor.WindowBackground

    default property alias content: container.data
    property alias inputParamsModel: lsidebar.inputParamsModel
    property alias outputParamsModel: lsidebar.outputParamsModel

    header: T_CVHeader {
        width: parent.width
        height: 40
    }

    QuickSplitView {
        id: splitView
        anchors.fill: parent
        T_CVLSidebar {
            id: lsidebar
            border.color: window.color
            SplitView.minimumWidth: 256
            SplitView.preferredWidth: 321
            SplitView.fillHeight: true
            inputParamsModel: _inputParams
            outputParamsModel: _outputParams
//            outputParamsModel: ListModel {
//                dynamicRoles: true
//                ListElement { paramName: "状态"; paramType: 10000; paramTypeName: "整型"; paramValue: 0 }
//                ListElement { paramName: "状态信息"; paramType: 1; paramTypeName: "字符串"; paramValue: "运行成功" }
////                ListElement { paramName: "运行时间"; paramType: 2; paramTypeName: "一维浮点数组"; paramValue: "[0.1, 0.05]" }
//            }
        }
        Item {
            id: container
            SplitView.fillHeight: true
            SplitView.fillWidth: true
        }
    }

    footer: T_CVFooter {
        width: parent.width
        height: 40
    }

    ListModel {
        id: _inputParams
        dynamicRoles: true
    }
    ListModel {
        id: _outputParams
        dynamicRoles: true
    //                ListElement { paramName: "状态"; paramType: 10000; paramTypeName: "整型"; paramValue: 0 }
    //                ListElement { paramName: "状态信息"; paramType: 1; paramTypeName: "字符串"; paramValue: "运行成功" }
    ////                ListElement { paramName: "运行时间"; paramType: 2; paramTypeName: "一维浮点数组"; paramValue: "[0.1, 0.05]" }
    }

    Component.onCompleted: {
        _inputParams.append({paramName: "图像", paramType: 20000, paramTypeName: "图像", paramValue: "F:/data/VOC/VOC2007/val/images/008258.jpg"})
        _outputParams.append({paramName: "状态", paramType: 10000, paramTypeName: "整型", paramValue: 0})
        _outputParams.append({paramName: "状态信息", paramType: 1, paramTypeName: "字符串", paramValue: "运行成功"})
    }
}
