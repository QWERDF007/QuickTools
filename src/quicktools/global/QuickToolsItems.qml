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
        count: RecentlyItems.getRecentlyAddedData().length + RecentlyItems.getRecentlyUpdatedData().length
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
        url: "qrc:/qt/qml/QuickTools/page/ImageProcessPage.qml"
        onTap: { navigationView.push(url) }
    }

    QuickPaneItemExpander {
        id: dlgroup
        title: "深度学习"
        iconVisible: true
        icon: QuickFontIcon.ReportHacked
        group: QuickToolManager.getGroupUUID(QuickToolGroupType.DeepLearningGroup)
        QuickPaneItem {
            title: qsTr("图像分类")
            url: "qrc:/qt/qml/QuickTools/page/ClassificationPage.qml"
            onTap: { navigationView.push(url) }
            group: dlgroup.group
        }
        QuickPaneItem {
            title: qsTr("目标检测")
            url: "qrc:/qt/qml/QuickTools/page/DetectionPage.qml"
            onTap: { navigationView.push(url) }
            group: dlgroup.group
        }
        QuickPaneItem {
            title: qsTr("语义分割")
            url: "qrc:/qt/qml/QuickTools/page/SegmentationPage.qml"
            onTap: { navigationView.push(url) }
            group: dlgroup.group
        }
    }

    QuickPaneItem {
        title: qsTr("示例工具")
        group: QuickToolManager.getGroupUUID(QuickToolGroupType.SamplesGroup)
        iconVisible: true
        icon: QuickFontIcon.Emoji2
        url: "qrc:/qt/qml/QuickTools/page/SamplesPage.qml"
        onTap: { navigationView.push(url) }
    }

    function startPageByItem(data){
        navigationView.startPageByItem(data)
    }
}
