import QtQuick
import QtQuick.Controls

import QuickTools.ui

QuickControlBackground {
    property Item inputItem
    id:control
    color: {
        if(inputItem && inputItem.disabled){
            return Qt.rgba(252/255,252/255,252/255,1)
        }
        if(inputItem && inputItem.activeFocus){
            return Qt.rgba(1,1,1,1)
        }
        if(inputItem && inputItem.hovered){
            return Qt.rgba(251/255,251/255,251/255,1)
        }
        return Qt.rgba(1,1,1,1)
    }
    border.width: 1
    gradient: Gradient {
        GradientStop { position: 0.0; color: d.startColor }
        GradientStop { position: 1 - d.offsetSize/control.height; color: d.startColor }
        GradientStop { position: 1.0; color: d.endColor }
    }
    bottomMargin: inputItem && inputItem.activeFocus ? 2 : 1
    QtObject{
        id:d
        property int offsetSize  : inputItem && inputItem.activeFocus ? 2 : 3
        property color startColor: Qt.rgba(232/255,232/255,232/255,1)
        property color endColor: {
            if(!control.enabled){
                return d.startColor
            }
            return inputItem && inputItem.activeFocus ? QuickColor.Primary : Qt.rgba(132/255,132/255,132/255,1)
        }
    }
}
