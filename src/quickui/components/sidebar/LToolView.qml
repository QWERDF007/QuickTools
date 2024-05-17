import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

StackLayout {
    property alias helpInfos: _helpInfos.text
    property alias inputParams: inputParamsListView.model
    property color itemBackgroundColor: QuickColor.White
    property color itemBorderColor: QuickColor.WindowBackground
    property bool itemEnable: true
    property alias outputParams: outputParamsListView.model

    height: 200
    width: 256

    ParamsListView {
        id: inputParamsListView

        Layout.fillHeight: true
        Layout.fillWidth: true
        paramItemEnable: itemEnable
    }
    ParamsListView {
        id: outputParamsListView

        Layout.fillHeight: true
        Layout.fillWidth: true
        paramItemEnable: itemEnable
    }
    HelpInfos {
        id: _helpInfos

        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
