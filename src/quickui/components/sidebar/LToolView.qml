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
    property alias inputParams: inputParamsListView.model
    property alias outputParams: outputParamsListView.model

    ParamsListView {
        id: inputParamsListView
        Layout.fillHeight: true
        Layout.fillWidth: true
    }

    ParamsListView {
        id: outputParamsListView
        Layout.fillHeight: true
        Layout.fillWidth: true
    }

    Item {
        id: helpInfos
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
