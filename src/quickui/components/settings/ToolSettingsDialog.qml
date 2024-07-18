import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: dialog
    title: qsTr("设置")
    property alias settingsModel: toolsettingsview.settingsModel
    property alias negativeText: toolsettingsview.negativeText
    property alias positiveText: toolsettingsview.positiveText
    property alias onNegativeClickListener: toolsettingsview.onNegativeClickListener
    property alias onPositiveClickListener: toolsettingsview.onPositiveClickListener
    signal negativeClicked
    signal positiveClicked
    ToolSettingsView {
        id: toolsettingsview
        anchors.fill: parent
        onNegativeClicked: dialog.negativeClicked()
        onPositiveClicked: dialog.positiveClicked()
    }
}
