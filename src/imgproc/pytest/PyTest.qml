import QtQuick
//import QtQuick.Controls
import QuickTools.ui
import QuickTools.core

T_CVWindow {
    id: pytestWindow
    quicktool: QuickToolFactor.createQuickTool(QuickToolType.PyTestTool, pytestWindow)
    Component.onCompleted: {
        console.log("quicktool", quicktool)
        console.log("hasPython", quicktool.hasPython)
        quicktool.reloadModule()
    }
}
