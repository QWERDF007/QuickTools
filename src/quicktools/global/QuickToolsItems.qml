pragma Singleton

import QtQuick
import QtQuick.Controls

import QuickTools.ui
import QuickTools.core

QuickObject {

    property var navigationView
    property var paneItemMenu


    QuickPaneItem {
        id:item_home
        count: QuickToolManager.getRecentlyChangedToolsCount()
        title: "Home"
        //        menuDelegate: paneItemMenu
        infoBadge: QuickBadge {
            count: item_home.count
        }
        icon: QuickFontIcon.Home
        url:"qrc:/qt/qml/QuickTools/page/Home.qml"
        onTap: {
            if(navigationView.getCurrentUrl()){
                item_home.count = 0
            }
            navigationView.push(url)
        }
    }

    QuickPaneItem {
        title: qsTr("图像处理")
        group: QuickToolManager.getGroupUUID(QuickToolGroupType.ImageProcessGroup)
        iconVisible: true
        icon: QuickFontIcon.QRCode
        url: "qrc:/qt/qml/QuickTools/page/T_Page.qml"
        onTap: { navigationView.push(url); navigationView.contentLoader.item.model = QuickToolManager.getToolsConfig(group, task); }
    }

    QuickPaneItemExpander {
        id: dlgroup
        title: "深度学习"
        iconVisible: true
        icon: QuickFontIcon.ReportHacked
        group: QuickToolManager.getGroupUUID(QuickToolGroupType.DeepLearningGroup)
        QuickPaneItem {
            title: qsTr("图像分类")
            url: "qrc:/qt/qml/QuickTools/page/T_Page.qml"
            onTap: { navigationView.push(url); navigationView.contentLoader.item.model = QuickToolManager.getToolsConfig(group, task); }
            group: dlgroup.group
        }
        QuickPaneItem {
            title: qsTr("目标检测")
            url: "qrc:/qt/qml/QuickTools/page/T_Page.qml"
            group: dlgroup.group
            task: QuickToolManager.getTaskUUID(QuickToolTaskType.DetectionTask)
            onTap: { navigationView.push(url); navigationView.contentLoader.item.model = QuickToolManager.getToolsConfig(group, task); }
        }
        QuickPaneItem {
            title: qsTr("语义分割")
            url: "qrc:/qt/qml/QuickTools/page/T_Page.qml"
            group: dlgroup.group
            task: QuickToolManager.getTaskUUID(QuickToolTaskType.EmptyTask)
            onTap: { navigationView.push(url); navigationView.contentLoader.item.model = QuickToolManager.getToolsConfig(group, task); }
        }
    }

    QuickPaneItem {
        title: qsTr("示例工具")
        iconVisible: true
        icon: QuickFontIcon.Emoji2
        url: "qrc:/qt/qml/QuickTools/page/T_Page.qml"
        group: QuickToolManager.getGroupUUID(QuickToolGroupType.SamplesGroup)
        onTap: { navigationView.push(url); navigationView.contentLoader.item.model = QuickToolManager.getToolsConfig(group, task); }
    }

    function startPageByItem(data){
        navigationView.startPageByItem(data)
    }
}
