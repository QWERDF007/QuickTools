import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui
import QuickTools.core
import "./settingitem"

QuickFrame {
    id: settingDelegate

    property var settingAdditional
    property string settingDesc: ""
    property string settingDisplayName: ""
    property int settingType: -1
    property var settingValue

    signal valueChanged(var value)

    height: 48
    width: 200

    Component {
        id: toggleSwitchSettingItem

        ToggleSwitchSettingItem {
            settingAdditional: settingDelegate.settingAdditional
            settingValue: settingDelegate.settingValue

            onValueChanged: function (value) {
                settingDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: sliderSettingItem

        SliderSettingItem {
            settingAdditional: settingDelegate.settingAdditional
            settingValue: settingDelegate.settingValue

            onValueChanged: function (value) {
                settingDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: colorSettingItem

        ColorSettingItem {
            settingAdditional: settingDelegate.settingAdditional
            settingValue: settingDelegate.settingValue

            onValueChanged: function (value) {
                settingDelegate.valueChanged(value)
            }
        }
    }
    Component {
        id: intInputSettingItem

        IntInputSettingItem {
            settingAdditional: settingDelegate.settingAdditional
            settingValue: settingDelegate.settingValue

            onValueChanged: function (value) {
                settingDelegate.valueChanged(value)
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 5

            Item {
                Layout.fillHeight: true
            }
            QuickText {
                font: QuickFont.Body
                text: settingDelegate.settingDisplayName
            }
            QuickText {
                font: QuickFont.Caption
                textColor: QuickColor.FontCaption
                text: settingDelegate.settingDesc
                visible: settingDesc ? true : false
            }
            Item {
                Layout.fillHeight: true
            }
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        QuickLoader {
            id: loader

            Layout.fillHeight: true
            sourceComponent: {
                if (settingDelegate.settingType === QuickToolSettings.ToggleSwitchType) {
                    return toggleSwitchSettingItem
                } else if (settingDelegate.settingType === QuickToolSettings.SliderType) {
                    return sliderSettingItem
                } else if (settingDelegate.settingType === QuickToolSettings.ColorDialogType) {
                    return colorSettingItem
                } else if (settingDelegate.settingType === QuickToolSettings.IntInputType) {
                    return intInputSettingItem
                }

                return undefined
            }
        }
    }
}
