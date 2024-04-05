import QtQuick
import QtQuick.Controls

import QuickTools.ui

Item {
    id: app

    Component.onCompleted: {
        QuickApp.init(app)
        var args = Qt.application.arguments
        QuickApp.navigate("qrc:/qt/qml/QuickTools/qml/window/MainWindow.qml")
    }
}
