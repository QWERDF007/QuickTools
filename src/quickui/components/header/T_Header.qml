import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

Rectangle {
    id: header

    property bool hasPython: false
    property color itemDisableColor: QuickColor.ItemDisabled
    property color toolbarBorderColor: QuickColor.WindowBackground
    property var toolbarButtons: Component {
        // 默认内容代理为空, 可重载
        Item {
        }
    }
    property var rightToolBarButtons: Component {
        // 默认内容代理为空, 可重载
        Item {
        }
    }
    property color toolbarColor: QuickColor.White

    signal settingsBtnClicked
    signal startBtnClicked
    signal reloadBtnClicked

    height: 40
    width: 200

    Rectangle {
        id: toolbar

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        border.color: toolbarBorderColor
        border.width: 2
        color: header.enabled ? toolbarColor : itemDisableColor
        height: 36
        implicitWidth: childrenRect.width
        radius: 3

        RowLayout {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 1

            QuickToolButton {
                id: playBtn

                disableColor: itemDisableColor
                icon.source: "/icons/play"
                implicitHeight: 32
                implicitWidth: 32

                onClicked: startBtnClicked()

                QuickToolTip {
                    delay: 500
                    text: qsTr("运行 F5")
                    visible: playBtn.hovered
                }
            }
            Rectangle {
                Layout.fillHeight: true // 分割线
                color: toolbarBorderColor
                width: 3
            }
            RowLayout {
                spacing: 0

                QuickLoader {
                    sourceComponent: header.visible ? header.toolbarButtons : undefined
                }
            }
        }
    }
    Component {
        id: reloadComponet
        QuickToolButton {
            id: reloadBtn
            disableColor: itemDisableColor
            icon.source: "/icons/reset"
            implicitHeight: 32
            implicitWidth: 32

            onClicked: reloadBtnClicked()

            QuickToolTip {
                delay: 500
                text: qsTr("刷新Python")
                visible: reloadBtn.hovered
            }
        }
    }

    RowLayout {
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        spacing: 1

        QuickLoader {
            sourceComponent: header.visible ? header.rightToolBarButtons : undefined
        }

        QuickLoader {
            sourceComponent: hasPython ? reloadComponet : undefined
        }

        Rectangle {
            visible: hasPython
            Layout.fillHeight: true // 分割线
            color: toolbarBorderColor
            width: 3
        }

        QuickToolButton {
            id: settingsBtn
            disableColor: itemDisableColor
            icon.source: "/icons/settings"
            implicitHeight: 32
            implicitWidth: 32

            onClicked: settingsBtnClicked()

            QuickToolTip {
                delay: 500
                text: qsTr("设置")
                visible: settingsBtn.hovered
            }
        }
    }
}
