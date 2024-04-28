import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

StackLayout {
    width: 256
    height: 200

    property color itemBackgroundColor: QuickColor.White
    property color itemBorderColor: QuickColor.WindowBackground
    property alias inputParams: inputParamsListView.model
    property alias outputParams: outputParamsListView.model
    property alias helpInfos: _helpInfos.text
    property bool itemEnable: true

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

    ToolSettingsView {
        id: toolSettingsView
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
