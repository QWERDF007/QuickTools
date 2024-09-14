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

    Component {
        id: rect_com
        QuickRectangle {
            x: shapeData[0]
            y: shapeData[1]
            width: shapeData[2]
            height: shapeData[3]
            color: control.color
            border.color: control.borderColor
            border.width: 3
        }
    }

    Component {
        id: cricle_com
        QuickCircle {
            center.x: shapeData[0]
            center.y: shapeData[1]
            radius: shapeData[2]
            color: control.color
            border.color: control.borderColor
            border.width: 3
        }
    }

    Component {
        id: polygon_com
        QuickCircle {
            center.x: shapeData[0]
            center.y: shapeData[1]
            radius: shapeData[2]
            color: control.color
            border.color: control.borderColor
            border.width: 3
        }
    }


    property color color: "transparent"
    property color borderColor: "yellow"
    delegate: QuickLoader {
//        anchors.fill: parent
        property var shapeData: model.shapeData
        property var shapeType: model.shapeType
        sourceComponent: {
            if (shapeType === QuickShape.Rectangle) {
                return rect_com
            } else if (shapeType === QuickShape.Circle) {
                return circle_com
            } else if (shapeType === QuickShape.Polygon) {
                return polygon_com
            }
            return undefined
        }
    }
}
