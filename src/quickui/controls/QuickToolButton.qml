import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Templates as T
import QuickTools.ui

T.ToolButton {
    id: control
    property color disableColor: QuickColor.Grey50
    property alias bg: bg
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 6

    icon.width: 24
    icon.height: 24
    icon.color: visualFocus ? control.palette.highlight : control.palette.buttonText

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: control.visualFocus ? control.palette.highlight : control.palette.buttonText
    }

    background: Rectangle {
        id: bg
        implicitWidth: 40
        implicitHeight: 40
        border.color: QuickColor.Grey80
        border.width: control.enabled && control.hovered ? 1 : 0

        // opacity: control.down ? 1.0 : 0.5
        // color: control.down || control.checked || control.highlighted ? QuickColor.ItemCheck : QuickColor.ItemNormal // control.palette.button
        color: {
            if (!control.enabled) {
                return disableColor
            }
            else if (control.down || control.checked || control.highlighted) {
                return QuickColor.ItemCheck
            } else if (control.hovered) {
                return QuickColor.ItemHover
            }
            return QuickColor.ItemNormal
        }
    }
}
