import QtQuick
import QtQuick.Controls

import QuickTools.ui

Item {
    id: app

    Component.onCompleted: {
        App.init(app)
        App.navigate("qrc:/qt/qml/QuickTools/window/MainWindow.qml")
    }
}
