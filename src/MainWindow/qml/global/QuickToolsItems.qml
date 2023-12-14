pragma Singleton

import QtQuick
import QtQuick.Controls

import QuickTools.ui

QuickObject {

    property var navigationView
    property var paneItemMenu


//    QuickPaneItem{
//        id:item_home
//        count: 9
//        title:Lang.home
//        menuDelegate: paneItemMenu
//        infoBadge:FluBadge{
//            count: item_home.count
//        }
//        icon:FluentIcons.Home
//        url:"qrc:/example/qml/page/T_Home.qml"
//        onTap:{
//            if(navigationView.getCurrentUrl()){
//                item_home.count = 0
//            }
//            navigationView.push(url)
//        }
//    }



    QuickPaneItemExpander {
        title: "图像处理"
        iconVisible: true
        icon: QuickFontIcon.QRCode
    }

    QuickPaneItemExpander {
        title: "深度学习"
        iconVisible: true
        icon: QuickFontIcon.CheckboxComposite
    }
}
