import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

Item {
    id: control
    property alias settingsModel: settingsView.model

    property string negativeText: qsTr("关闭")
    property string positiveText: qsTr("保存")
    property var onNegativeClickListener
    property var onPositiveClickListener
    signal negativeClicked
    signal positiveClicked
    property bool useNegativeButton: true
    property bool usePositiveButton: true

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: settingsView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 5
            Layout.topMargin: 5

            boundsBehavior: Flickable.StopAtBounds
            clip: true
            spacing: 5

            ScrollBar.vertical: QuiScrollBar {}

            delegate: SettingItemDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 5
                height: 48
                settingAdditional: model.additional
                settingDesc: model.desc
                settingDisplayName: model.displayName
                settingType: model.type
                settingValue: model.value

                onValueChanged: function (value) {
                    model.value = value
                }
            }

            section {
                criteria: ViewSection.FullString
                property: "groupName"

                delegate: Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    height: 48

                    QuiText {
                        font: QuiFont.Title
                        text: section
                        color: QuiColor.FontPrimary
                    }
                }
            }
        }

        Rectangle { // 操作按钮布局
            id: layout_actions
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            radius: 5
            color: QuiColor.WindowBackground
            RowLayout{
                anchors.margins: spacing
                anchors.fill: parent
                spacing: 10
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 128
                    QuiButton { // 关闭按钮
                        id: negative_btn
                        enabled: settingsView.count > 0
                        width: parent.width
                        anchors.centerIn: parent
                        visible: control.useNegativeButton
                        text: negativeText
                        onClicked: {
                            if (control.onNegativeClickListener) {
                                control.onNegativeClickListener()
                            } else {
                                control.negativeClicked()
                            }
                        }
                    }
                }
                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 128
                    QuiFilledButton { // 保存按钮
                        id: positive_btn
                        enabled: settingsView.count > 0
                        width: parent.width
                        anchors.centerIn: parent
                        visible: control.usePositiveButton
                        text: positiveText
                        onClicked: {
                            if (control.onPositiveClickListener) {
                                control.onPositiveClickListener()
                            } else {
                                control.positiveClicked()
                            }
                        }
                    }
                }
            }
        }
    }
}
