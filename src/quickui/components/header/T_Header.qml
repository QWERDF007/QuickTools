import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

Rectangle {
    id: header

    property bool hasPython: false
    property color itemDisableColor: QuiColor.ItemDisabled
    property color toolbarBorderColor: QuiColor.WindowBackground
    property color toolbarColor: QuiColor.CardBackground
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

    signal settingsBtnClicked
    signal startBtnClicked
    signal reloadBtnClicked

    color: QuiColor.WindowBackground
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
        width: implicitWidth
        radius: 3

        RowLayout {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 1

            QuiToolButton {
                id: playBtn

                disableColor: itemDisableColor
                icon.source: "/icons/play"
                implicitHeight: 32
                implicitWidth: 32

                onClicked: startBtnClicked()

                QuiToolTip {
                    delay: 500
                    text: qsTr("运行 F5")
                    visible: playBtn.hovered
                }
            }
            Rectangle {
                Layout.fillHeight: true
                color: toolbarBorderColor
                width: 3
            }
            RowLayout {
                spacing: 0

                Loader {
                    sourceComponent: header.visible ? header.toolbarButtons : undefined
                }
            }
        }
    }

    Component {
        id: reloadComponet
        QuiToolButton {
            id: reloadBtn
            disableColor: itemDisableColor
            icon.source: "/icons/reset"
            implicitHeight: 32
            implicitWidth: 32

            onClicked: reloadBtnClicked()

            QuiToolTip {
                delay: 500
                text: qsTr("刷新Python")
                visible: reloadBtn.hovered
            }
        }
    }

    RowLayout {
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        spacing: 1

        Loader {
            sourceComponent: header.visible ? header.rightToolBarButtons : undefined
        }

        Loader {
            sourceComponent: hasPython ? reloadComponet : undefined
        }

        Rectangle {
            visible: hasPython
            Layout.fillHeight: true
            color: toolbarBorderColor
            width: 3
        }

        QuiToolButton {
            id: settingsBtn
            disableColor: itemDisableColor
            icon.source: "/icons/settings"
            implicitHeight: 32
            implicitWidth: 32

            onClicked: settingsBtnClicked()

            QuiToolTip {
                delay: 500
                text: qsTr("设置")
                visible: settingsBtn.hovered
            }
        }
    }
}
