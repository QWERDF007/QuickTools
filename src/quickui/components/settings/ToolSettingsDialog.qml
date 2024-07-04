import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

Window {
    id: control
    property alias settingsModel: settingsView.model

    property string negativeText: qsTr("关闭")
    property string positiveText: qsTr("保存")
    property var onNegativeClickListener
    property var onPositiveClickListener
    signal negativeClicked
    signal positiveClicked

    title: qsTr("设置")
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

            ScrollBar.vertical: QuickScrollBar {}

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

                    QuickText {
                        font: QuickFont.Title
                        text: section
                    }
                }
            }
        }

        Rectangle { // 操作按钮布局
            id:layout_actions
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            radius: 5
            color: Qt.rgba(243/255,243/255,243/255,1)
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
                    QuickButton { // 关闭按钮
                        id: negative_btn
                        width: parent.width
                        anchors.centerIn: parent
                        visible: true // control.buttonFlags&QuickDialogButtonFlag.NeutralButton
                        text: negativeText
                        onClicked: {
                            if (control.onNegativeClickListener) {
                                control.onNegativeClickListener()
                            } else {
                                negativeClicked()
                                control.close()
                            }
                        }
                    }
                }
                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 128
                    QuickFilledButton { // 保存按钮
                        id:positive_btn
                        width: parent.width
                        anchors.centerIn: parent
                        visible: true // control.buttonFlags&QuickDialogButtonFlag.PositiveButton
                        text: positiveText
                        onClicked: {
                            if (control.onPositiveClickListener) {
                                control.onPositiveClickListener()
                            } else {
                                positiveClicked()
                                control.close()
                            }
                        }
                    }
                }
            }
        }
    }
}
