import QtQuick
import QtQuick.Controls
import QuickTools.ui

Repeater {
    id: control

    // property var data
    // onDataChanged: {
    //     console.log("onDataChanged", data)
    //     console.log("objToString")
    //     console.log(objToString(data))
    // }


    /**
     * @brief 获取 obj 的属性并转换为字符串, 包括 function 和 property
     * @param obj
     */
    function objToString (obj) {
        var str = '';
        for (var p in obj) {
            if (Object.prototype.hasOwnProperty.call(obj, p)) {
                str += p + '::' + obj[p] + '\n';
            }
        }
        return str;
    }

    property color color: "transparent"
    property color borderColor: "yellow"
    delegate: QuickRectangle {
        property var data: model.shapeData
        x: data[0]
        y: data[1]
        width: data[2]
        height: data[3]
        color: control.color
        border.color: control.borderColor
        border.width: 3
    }
}
