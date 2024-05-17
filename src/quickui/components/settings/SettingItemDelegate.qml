import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

QuickFrame {
    property string desc: ""
    property string name: ""
    property int settingType: -1

    height: 32
    width: 200

    RowLayout {
        anchors.fill: parent

        Column {
            Layout.fillWidth: true

            QuickText {
                font: QuickFont.BodyStrong
                text: name
            }
            QuickText {
                font: QuickFont.Body
                text: desc
            }
        }
        QuickLoader {
            id: loader

        }
    }
}
