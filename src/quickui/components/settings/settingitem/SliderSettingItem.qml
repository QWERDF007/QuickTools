import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QuickTools.ui

T_SettingItem {
    id: sliderSettingItem

    RowLayout {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        QuickText {
            Layout.minimumWidth: 48
            horizontalAlignment: Text.AlignRight
            text: settingValue
        }
        QuickSlider {
            from: settingAdditional.from
            stepSize: settingAdditional.stepSize
            to: settingAdditional.to
            value: settingValue

            onMoved: {
                sliderSettingItem.valueChanged(value)
            }
        }
    }
}
