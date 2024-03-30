import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core

StackLayout {
    width: 256
    height: 200

    property color itemBackgroundColor: QuickColor.White
    property color itemBorderColor: QuickColor.WindowBackground
    property InputParams inputParams
    property OutputParams outputParams

    ParamsListView {
        id: inputParamsListView
        Layout.fillHeight: true
        Layout.fillWidth: true
        toolParams: inputParams
    }

    ParamsListView {
        id: outputParamsListView
        Layout.fillHeight: true
        Layout.fillWidth: true
        toolParams: outputParams
    }

    Item {
        id: helpInfos
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
