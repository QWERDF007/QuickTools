import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

Rectangle {
    id: header
    width: 200
    height: 40
    
    property color toolbarColor: QuickColor.White
    property color toolbarBorderColor: QuickColor.WindowBackground
    property color itemDisableColor: QuickColor.ItemDisabled
    signal startBtnClicked
    signal settingsBtnClicked

    property var toolbarButtons:  Component{ // 默认内容代理为空, 可重载
        Item{
        }
    }

    Rectangle {
        id: toolbar
        color: header.enabled ? toolbarColor : itemDisableColor
        radius: 3
        border.width: 2
        border.color: toolbarBorderColor
        height: 36
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        implicitWidth: childrenRect.width
        RowLayout {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 1
            QuickToolButton {
                id: playBtn
                implicitWidth: 32
                implicitHeight: 32
                disableColor: itemDisableColor
                icon.source: "/icons/play"
                onClicked: startBtnClicked()

                QuickToolTip {
                    visible: playBtn.hovered
                    text: qsTr("Run F5")
                    delay: 500
                }
            }
            Rectangle { // 分割线
                color: toolbarBorderColor
                width: 3
                Layout.fillHeight: true
            }

            RowLayout {
                id: btnRowLayout
                spacing: 0
                QuickLoader {
                    sourceComponent: header.visible ? header.toolbarButtons : undefined
                }
            }
        }
    }

    QuickToolButton {
        id: settingsBtn
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        implicitWidth: 32
        implicitHeight: 32
        disableColor: itemDisableColor
        icon.source: "/icons/settings"
        onClicked: settingsBtnClicked()

        QuickToolTip {
            visible: settingsBtn.hovered
            text: qsTr("Settings")
            delay: 500
        }
    }
}
