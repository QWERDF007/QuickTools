import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Templates as T

import QuickTools.ui


T.ItemDelegate {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)
    padding: 0
    verticalPadding: 8
    horizontalPadding: 10
    icon.color: control.palette.text
    contentItem: QuickText {
        text: control.text
        font: control.font
        color:{
            if(control.down){
                return QuickColor.Grey120
            }
            return QuickColor.Grey220
        }
    }
    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 30
        color: Qt.rgba(0,0,0,0.05)
        visible: control.down || control.highlighted || control.visualFocus
    }
}
