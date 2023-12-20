﻿pragma Singleton

import QtQuick
import QtQuick.Controls

import QuickTools.ui

QuickObject {

    property var navigationView
    property var paneItemMenu


    QuickPaneItem {
        id:item_home
        count: 9
        title: "Home"
//        menuDelegate: paneItemMenu
        infoBadge: QuickBadge {
            count: item_home.count
        }
        icon: QuickFontIcon.Home
        url:"qrc:/qt/qml/QuickTools/qml/page/T_Home.qml"
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
        url:"qrc:/qt/qml/QuickTools/qml/page/ImageProcessPage.qml"
        onTap: { navigationView.push(url) }
    }

    QuickPaneItemExpander {
        title: "深度学习"
        iconVisible: true
        icon: QuickFontIcon.ReportHacked
        QuickPaneItem {
            title: "图像分类"
            onTap: { console.log("未设置触发页面") }
        }
        QuickPaneItem {
            title: "语义分割"
            onTap: { console.log("未设置触发页面") }
        }
    }

    QuickPaneItemExpander {
        title: "未知"
        iconVisible: true
        icon: QuickFontIcon.CheckboxComposite
        QuickPaneItem {
            title: "AAA"
        }
        QuickPaneItem {
            title: "BBB"
        }
    }
}
