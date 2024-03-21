import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    width: 256
    height: 200

    property alias inputParamsModel: inputParamsListView.model
    property alias outputParamsModel: outputParamsListView.model

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
