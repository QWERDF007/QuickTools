import QtQuick
import QtQuick.Controls

import QuickTools.ui

Item {
    id: app

    Component.onCompleted: {
        QuickApp.init(app)
        QuickApp.navigate("qrc:/qt/qml/QuickTools/window/MainWindow.qml")
    }
}
