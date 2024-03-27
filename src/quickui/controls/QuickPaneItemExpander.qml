import QtQuick
import QtQuick.Controls

QtObject {
    id: control
    default property list<QtObject> children
    property int _idx
    property string title
    property var icon
    property bool disabled: false
    property bool iconVisible: true
    property bool isExpand: false
    property bool showEdit
    property Component iconDelegate
    property Component menuDelegate
    property Component editDelegate
}
