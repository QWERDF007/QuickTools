import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property var settingAdditional
    property var settingValue

    signal valueChanged(var value)

    anchors.bottom: parent.bottom
    anchors.top: parent.top
    width: childrenRect.width
}
