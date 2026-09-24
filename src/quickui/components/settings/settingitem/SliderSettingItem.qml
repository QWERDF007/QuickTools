import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import quickui
import QuickTools.ui

T_SettingItem {
    id: sliderSettingItem

    RowLayout {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        QuiText {
            Layout.minimumWidth: 48
            horizontalAlignment: Text.AlignRight
            text: settingValue !== undefined && settingValue !== null ? String(settingValue) : "0"
            font: QuiFont.Caption
            color: QuiColor.FontPrimary
        }
        QuiSlider {
            from: settingAdditional && settingAdditional.from !== undefined ? settingAdditional.from : 0
            stepSize: settingAdditional && settingAdditional.stepSize !== undefined ? settingAdditional.stepSize : 1
            to: settingAdditional && settingAdditional.to !== undefined ? settingAdditional.to : 100
            value: settingValue !== undefined && settingValue !== null ? Number(settingValue) : 0

            onMoved: {
                sliderSettingItem.valueChanged(value)
            }
        }
    }
}
