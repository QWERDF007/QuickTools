import QtQuick
import QtQuick.Controls

import QuickTools.ui
import QuickTools.core

QuickContentPage {
    ListView {
        anchors.fill: parent
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
}
