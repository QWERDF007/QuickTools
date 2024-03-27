import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core

Item {
    id: paramHeader
    width: 200
    height: 24
    property int paramType: -1
    property var paramValue

    Component{
        id: intItem
        Item{
            anchors.fill: parent
            clip: true
            QuickText {
                anchors.fill: parent
                text: paramHeader.paramValue === null || paramHeader.paramValue === undefined ? "" : paramHeader.paramValue
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    Component {
        id: textItem
        Item {
            anchors.fill: parent
            clip: true
            QuickText {
                anchors.fill: parent
                text: paramHeader.paramValue === null || paramHeader.paramValue === undefined ? "" : paramHeader.paramValue
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    Component{
        id: inputImageItem
        Item{
            anchors.fill: parent
            clip: true
            RowLayout{
                anchors.fill: parent
                QuickText{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    text: paramHeader.paramValue === null || paramHeader.paramValue === undefined ? "" : paramHeader.paramValue
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                spacing: 5
                ToolButton {
                    implicitWidth:  Math.min(parent.width, parent.height)
                    implicitHeight: Math.min(parent.width, parent.height)
                    icon.source: "/icons/image"
                }
            }
        }
    }


    QuickLoader {
        anchors.fill: parent
        sourceComponent: {
            if (paramHeader.paramType === QuickToolParam.ParamIntType) {
                return intItem
            } else if (paramHeader.paramType === QuickToolParam.ParamTextType) {
                return textItem
            } else if (paramHeader.paramType === QuickToolParam.ParamImageType) {
                return inputImageItem
            }
            return undefined
        }
    }
}
