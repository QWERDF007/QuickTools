pragma Singleton

import QtQuick
import QtQuick.Controls

import QuickTools.ui
import QuickTools.core

QuickObject {

    property var navigationView
    property var paneItemMenu

    QuickPaneItem {
        title: qsTr("工具管理")
        icon: QuickFontIcon.Manage
        url:"qrc:/qt/qml/QuickTools/page/ToolManagerPage.qml"
        onTap: {
            navigationView.push(url)
        }
    }

    QuickPaneItem {
        title: qsTr("设置")
        icon: QuickFontIcon.Settings
        url:"qrc:/qt/qml/QuickTools/page/SettingsPage.qml"
        onTap: {
            navigationView.push(url)
        }
    }

    QuickPaneItem {
        title: qsTr("关于")
        icon: QuickFontIcon.ContactInfo
        url:"qrc:/qt/qml/QuickTools/page/AboutPage.qml"
        onTap: {
            navigationView.push(url)
        }
    }
}
