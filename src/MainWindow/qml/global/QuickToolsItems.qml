pragma Singleton

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
        url:"qrc:/mainwindow/qml/page/ImageProcess.qml"
        onTap: {
            if(navigationView.getCurrentUrl()){
                item_home.count = 0
            }
            navigationView.push(url)
        }
    }


    QuickPaneItemExpander {
        title: "图像处理"
        iconVisible: true
        icon: QuickFontIcon.QRCode
        QuickPaneItem {
            id: item_111
            title: "AAA"
        }
        QuickPaneItem {
            id: item_112
            title: "BBB"
        }
    }

    QuickPaneItemExpander {
        title: "深度学习"
        iconVisible: true
        icon: QuickFontIcon.CheckboxComposite
        QuickPaneItem {
            id: item_221
            title: "AAA"
        }
        QuickPaneItem {
            id: item_222
            title: "BBB"
        }
    }
}
