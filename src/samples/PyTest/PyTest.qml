import QtQuick
import QuickTools.ui
import QuickTools.core

T_Window {
    id: pytestWindow
    quicktool: QuickToolFactor.createQuickTool(QuickToolType.PyTestTool, pytestWindow)
}
