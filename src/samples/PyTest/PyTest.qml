import QtQuick
import QuickTools.ui
import QuickTools.core

T_Window {
    id: pytestWindow
    quicktool: QuickToolManager.createQuickTool(QuickToolType.PyTestTool, pytestWindow)
}
