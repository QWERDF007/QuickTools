import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

StackLayout {
    width: 256
    height: 200

    property alias inputParamsModel: inputParamsListView.model
    property alias outputParamsModel: outputParamsListView.model
    property color itemBackgroundColor: QuickColor.White
    property color itemBorderColor: QuickColor.WindowBackground

    T_CVParamsListView {
        id: inputParamsListView
        Layout.fillHeight: true
        Layout.fillWidth: true
    }

    T_CVParamsListView {
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
