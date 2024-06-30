import QtQuick
import QtQuick.Controls

// 用于定义 T_Page 的项目列表
QtObject {
    id:control
    default property list<QtObject> children
    property string group
}
