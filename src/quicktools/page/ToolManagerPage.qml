import QtQuick
import QtQuick.Controls

import QuickTools.ui
import QuickTools.core


ListView {
    model: QuickToolManager.activatedTools
    delegate: Rectangle {
        height: 32
        width: childrenRect.width
        Row {
            QuickText {
                text: model.name
            }
            QuickText {
                text: model.uuid
            }
        }
    }
}
