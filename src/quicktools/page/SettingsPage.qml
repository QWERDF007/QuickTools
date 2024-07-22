import QtQuick
import QtQuick.Controls

import QuickTools.ui
import QuickTools.core

QuickContentPage {
    ToolSettingsView {
        anchors.fill: parent
        settingsModel: GlobalSettings
        buttonFlags: QuickDialogButtonFlag.PositiveButton
    }
}
