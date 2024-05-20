import QtQuick
import QtQuick.Controls
import QuickTools.ui

QtObject {
    property string key
    property int _idx
    property bool visible: true
    property string title
    property var parent
    Component.onCompleted: {
        key = UITools.uuid()
    }
}
