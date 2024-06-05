pragma Singleton

import QtQuick
import QtQuick.Controls

import QuickTools.ui

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
        // url: "qrc:/qt/qml/QuickTools/imgproc/imagehistogram/ImageHistogram.qml"
        // url: "qrc:/qt/qml/QuickTools/ui/window/T_CVWindow.qml"
        onTap: {
            if(navigationView.getCurrentUrl()){
                item_home.count = 0
            }
            navigationView.push(url)
        }
    }

    QuickPaneItem {
        title: "图像处理"
        iconVisible: true
        icon: QuickFontIcon.QRCode
        url: "qrc:/qt/qml/QuickTools/page/ImageProcessPage.qml"
        onTap: { navigationView.push(url) }
    }

    QuickPaneItemExpander {
        title: "深度学习"
        iconVisible: true
        icon: QuickFontIcon.ReportHacked
        QuickPaneItem {
            title: "图像分类"
            url: "qrc:/qt/qml/QuickTools/page/ImageClassificationPage.qml"
            onTap: { navigationView.push(url) }
        }
        QuickPaneItem {
            title: "语义分割"
            url: "qrc:/qt/qml/QuickTools/page/ImageSegmentationPage.qml"
            onTap: { navigationView.push(url) }
        }
    }

    QuickPaneItem {
        title: "示例工具"
        iconVisible: true
        icon: QuickFontIcon.Emoji2
        url: "qrc:/qt/qml/QuickTools/page/SamplesPage.qml"
        onTap: { navigationView.push(url) }
    }

    function startPageByItem(data){
        navigationView.startPageByItem(data)
    }
}
