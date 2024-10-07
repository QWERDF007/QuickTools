import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts

import QuickTools.ui
import QuickTools.core

Item {
    property alias contentLoader: loader_content
    property url logo
    property string title: ""
    property QuickObject items
    property QuickObject footerItems
    // property int displayMode: FluNavigationViewType.Auto
    property Component autoSuggestBox
    property Component actionItem
    property int topPadding: 0
    // property int pageMode: FluNavigationViewType.Stack
//    property QuickMenu navItemRightMenu
//    property QuickMenu navItemExpanderRightMenu
    property int navCompactWidth: 50
    property int navTopMargin: 0
    property int cellHeight: 38
    property int cellWidth: 300
    property bool hideNavAppBar: false
//    property alias buttonMenu: btn_menu
//    property alias buttonBack: btn_back
//    property alias imageLogo: image_logo
    signal logoClicked
    id:control
    Item {
        id:d
        property bool animDisabled:false
        property var stackItems: []
//        property int displayMode: control.displayMode
        property bool enableNavigationPanel: false
        property bool isCompact: false
        property bool isMinimal: false
        // property bool isCompact: d.displayMode === FluNavigationViewType.Compact
        // property bool isMinimal: d.displayMode === FluNavigationViewType.Minimal
        // property bool isCompactAndPanel: d.displayMode === FluNavigationViewType.Compact && d.enableNavigationPanel
        // property bool isCompactAndNotPanel:d.displayMode === FluNavigationViewType.Compact && !d.enableNavigationPanel
        // property bool isMinimalAndPanel: d.displayMode === FluNavigationViewType.Minimal && d.enableNavigationPanel
        property color itemDisableColor: Qt.rgba(160/255,160/255,160/255,1)
        // onIsCompactAndNotPanelChanged: {
        //     collapseAll()
        // }
        function handleItems(){
            var _idx = 0
            var data = []
            var comEmpty = Qt.createComponent("QuickPaneItemEmpty.qml");
            if(items){
                for(var i=0;i<items.children.length;i++){
                    var item = items.children[i]
                    if(item.visible !== true){
                        continue
                    }
                    item._idx = _idx
                    data.push(item)
                    _idx++
                    if(item instanceof QuickPaneItemExpander){
                        for(var j=0;j<item.children.length;j++){
                            var itemChild = item.children[j]
                            if(itemChild.visible !== true){
                                continue
                            }
                            itemChild._parent = item
                            itemChild._idx = _idx
                            data.push(itemChild)
                            _idx++
                        }
                    }
                }
                if(footerItems){ // 往 nav_list 添加空项, 方便使用统一的 push(url,args) 进行跳转
                    for(var k=0;k<footerItems.children.length;k++){
                        var itemFooter = footerItems.children[k]
                        if(itemFooter.visible !== true){
                            continue
                        }
                        var objEmpty = comEmpty.createObject(items,{_idx:_idx});
                        itemFooter._idx = _idx;
                        data.push(objEmpty)
                        _idx++
                    }
                }
            }
            return data
        }
        function handleFooterItems(){
            var data = []
            if(footerItems){
                for(var i=0;i<footerItems.children.length;i++){
                    var item = footerItems.children[i]
                    if(item.visible !== true){
                        continue
                    }
                    data.push(item)
                }
            }
            return data;
        }
    }
    //    Component.onCompleted: {
    //        d.displayMode = Qt.binding(function(){
    //            if(control.displayMode !==FluNavigationViewType.Auto){
    //                return control.displayMode
    //            }
    //            if(control.width<=700){
    //                return FluNavigationViewType.Minimal
    //            }else if(control.width<=900){
    //                return FluNavigationViewType.Compact
    //            }else{
    //                return FluNavigationViewType.Open
    //            }
    //        })
    //        timer_anim_delay.restart()
    //    }
    //    Timer{
    //        id:timer_anim_delay
    //        interval: 200
    //        onTriggered: {
    //            d.animDisabled = true
    //        }
    //    }
    //    Connections{
    //        target: d
    //        function onDisplayModeChanged(){
    //            if(d.displayMode === FluNavigationViewType.Compact){
    //                collapseAll()
    //            }
    //            d.enableNavigationPanel = false
    //            if(loader_auto_suggest_box.item){
    //                loader_auto_suggest_box.item.focus = false
    //            }
    //        }
    //    }
    Component{ // 空项
        id:com_panel_item_empty
        Item{
            visible: false
        }
    }


    Component{ // 分割线
        id:com_panel_item_separator
        QuickDivider{
            width: layout_list.width
            spacing: {
                if(model){
                    return model.spacing
                }
                return 1
            }
            size: {
                if(!model){
                    return 1
                }
                if(model._parent){
                    return model._parent.isExpand ? model.size : 0
                }
                return model.size
            }
        }
    }
    Component{
        id:com_panel_item_header
        Item{
            height: {
                if(model._parent){
                    return model._parent.isExpand ? control.cellHeight : 0
                }
                return  control.cellHeight
            }
            Behavior on height {
                enabled: true // FluTheme.animationEnabled && d.animDisabled
                NumberAnimation{
                    duration: 83
                }
            }
            width: layout_list.width
            QuickText{
                text:model.title
                font: QuickFont.BodyStrong
                anchors{
                    bottom: parent.bottom
                    left:parent.left
                    leftMargin: 10
                }
            }
        }
    }
    Component{ // 导航展开列表 expander
        id:com_panel_item_expander
        Item{
            height: control.cellHeight
            width: layout_list.width
            QuickControl {
                id: item_control
                enabled: !model.disabled
                anchors{
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 2
                    bottomMargin: 2
                    leftMargin: 6
                    rightMargin: 6
                }
                QuickToolTip {
                    text: model.title
                    visible: false // item_control.hovered && model.title && d.isCompactAndNotPanel
                    delay: 800
                }
                MouseArea { // 右键菜单
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onClicked:
                        (mouse) =>{
                            if (mouse.button === Qt.RightButton) {
                                if(model.menuDelegate){
                                    loader_item_menu.sourceComponent = model.menuDelegate
                                    connection_item_menu.target = loader_item_menu.item
                                    loader_item_menu.modelData = model
                                    loader_item_menu.item.popup();
                                }
                            }
                        }
                    z:-100
                }
                onClicked: { // 点击展开列表
                    model.isExpand = !model.isExpand
                }
                Rectangle { // 右上角提示圆点
                    color:Qt.rgba(255/255,77/255,79/255,1)
                    width: 10
                    height: 10
                    radius: 5
                    border.width: 1
                    border.color: Qt.rgba(1,1,1,1)
                    anchors{
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                        rightMargin: 3
                        verticalCenterOffset: -8
                    }
                    visible: {
                        if(!model){
                            return false
                        }
                        if(!model.isExpand){
                            for(var i=0;i<model.children.length;i++){
                                var item = model.children[i]
                                if(item.infoBadge && item.count !==0){
                                    return true
                                }
                            }
                        }
                        return false
                    }
                }
                Rectangle { // item 显示内容 (高亮+背景+图标+标题文字)
                    radius: 4
                    anchors.fill: parent
                    Rectangle { // 选中高亮竖线
                        width: 3
                        height: 18
                        radius: 1.5
                        color: QuickColor.Primary
                        visible: {
                            if(!model){
                                return false
                            }
                            if(!model.children){
                                return false
                            }
                            for(var i=0;i<model.children.length;i++){
                                var item = model.children[i]
                                if(item.visible !== true){
                                    continue
                                }
                                if(item._idx === nav_list.currentIndex && !model.isExpand){
                                    return true
                                }
                            }
                            return false
                        }
                        anchors{
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    QuickTextIcon { // 箭头图标
                        id: item_icon_expand
                        rotation: model&&model.isExpand?0:180
                        iconSource: QuickFontIcon.ChevronUp
                        iconSize: 15
                        anchors{
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                            rightMargin: visible ? 12 : 0
                        }
                        visible: true
                        Behavior on rotation {
                            NumberAnimation{
                                duration: 167
                                easing.type: Easing.OutCubic
                            }
                        }
                        color: {
                            if(!item_control.enabled){
                                return d.itemDisableColor
                            }
                            return "#000000"
                        }
                    }
                    color: { // item 不同状态下的背景色
                        if(!item_control.enabled) { // 禁用状态下的颜色
                            return QuickColor.ItemNormal
                        }
                        if(nav_list.currentIndex === _idx && type === 0){
                            return QuickColor.ItemCheck
                        }
                        if(item_control.hovered) { // 鼠标悬浮时的颜色
                            return QuickColor.ItemHover
                        }
                        return QuickColor.ItemNormal
                    }
                    Component{ // 图标组件
                        id:com_icon
                        QuickTextIcon{
                            iconSource: {
                                if(model&&model.icon){
                                    return model.icon
                                }
                                return 0
                            }
                            iconSize: 15
                            color: {
                                if(!item_control.enabled){
                                    return d.itemDisableColor
                                }
                                return "#000000"
                            }
                        }
                    }
                    Item{ // 图标容器
                        id:item_icon
                        width: visible ? 30 : 8
                        height: 30
                        visible: {
                            if(model){
                                return model.iconVisible
                            }
                            return true
                        }
                        anchors{
                            left:parent.left
                            verticalCenter: parent.verticalCenter
                            leftMargin: 3
                        }
                        QuickLoader{
                            anchors.centerIn: parent
                            sourceComponent: {
                                if(model&&model.iconDelegate){
                                    return model.iconDelegate
                                }
                                return com_icon
                            }
                        }
                    }
                    QuickText{ // 标题文字
                        id:item_title
                        text:{
                            if(model){
                                if(!item_icon.visible) {
                                    return model.title[0]
                                }
                                return model.title
                            }
                            return ""
                        }
                        visible: true
                        elide: Text.ElideRight
                        anchors{
                            verticalCenter: parent.verticalCenter
                            left:item_icon.right
                            right: item_icon_expand.left
                        }
                        color:{
                            if(!item_control.enabled){
                                return d.itemDisableColor
                            }
                            if(item_control.pressed){
                                return QuickColor.Grey120
                            }
                            return QuickColor.Grey220
                        }
                    }
                    QuickLoader {
                        id: item_edit_loader
                        anchors{
                            top: parent.top
                            bottom: parent.bottom
                            left: item_title.left
                            right: item_title.right
                            rightMargin: 8
                        }
                        sourceComponent: {
                            if(d.isCompact){
                                return undefined
                            }
                            return model&&model.showEdit ? model.editDelegate : undefined
                        }
                        onStatusChanged: {
                            if(status === QuickLoader.Ready){
                                item.forceActiveFocus()
                                item_connection_edit_focus.target = item
                            }
                        }
                        Connections{
                            id:item_connection_edit_focus
                            ignoreUnknownSignals:true
                            function onActiveFocusChanged(focus){
                                if(focus === false){
                                    model.showEdit = false
                                }
                            }
                            function onCommit(text){
                                model.title = text
                                model.showEdit = false
                            }
                        }
                    }
                }
            }
        }
    }
    Component{ // 导航项 item
        id:com_panel_item
        Item{
            Behavior on height {
                enabled: true // FluTheme.animationEnabled && d.animDisabled
                NumberAnimation{
                    duration: 167
                    easing.type: Easing.OutCubic
                }
            }
            height: {
                if(model&&model._parent){
                    return model._parent.isExpand ? control.cellHeight : 0
                }
                return control.cellHeight
            }
            visible: control.cellHeight === Number(height)
            opacity: visible
            Behavior on opacity {
                NumberAnimation { duration: 83 }
            }
            width: layout_list.width
            QuickControl {
                property var modelData: model
                id:item_control
                enabled: !model.disabled
                anchors{
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 2
                    bottomMargin: 2
                    leftMargin: 6
                    rightMargin: 6
                }
                QuickToolTip {
                    text: model.title
                    visible: false // item_control.hovered && model.title && d.isCompact
                    delay: 800
                }
                onClicked:{
                    if (type === 0) { // nav items
                        if(model.onTapListener){
                            model.onTapListener()
                        }else{
                            nav_list.currentIndex = _idx
                            layout_footer.currentIndex = -1 // 取消底部组件选中
                            model.tap()
                        }
                    } else { // footer items
                        if(model.onTapListener){
                            model.onTapListener()
                        }else{
                            nav_list.currentIndex = nav_list.count-layout_footer.count+_idx
                            layout_footer.currentIndex = _idx
                            model.tap()
                        }
                    }
                }
                MouseArea{
                    id:item_mouse
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton | Qt.LeftButton
                    onClicked:
                        (mouse)=>{
                            if (mouse.button === Qt.RightButton) {
                                if(model.menuDelegate){
                                    loader_item_menu.sourceComponent = model.menuDelegate
                                    connection_item_menu.target = loader_item_menu.item
                                    loader_item_menu.modelData = model
                                    loader_item_menu.item.popup();
                                }
                            }else{
                                item_control.clicked()
                            }
                        }
                }
                Rectangle { // item 显示内容 (背景+图标+标题文字)
                    radius: 4
                    anchors.fill: parent
                    color: {
                        if (!item_control.enabled) { // 禁用状态下的颜色
                            return QuickColor.ItemNormal
                        }
                        if(type===0){
                            if(nav_list.currentIndex === _idx) { // 选中颜色
                                return QuickColor.ItemCheck
                            }
                        }else{
                            if(nav_list.currentIndex === (nav_list.count-layout_footer.count+_idx)){
                                return QuickColor.ItemCheck
                            }
                        }
                        if(item_control.hovered) { // 鼠标悬浮时的颜色
                            return QuickColor.ItemHover
                        }
                        return QuickColor.ItemNormal
                    }
                    Component { // 图标组件
                        id:com_icon
                        QuickTextIcon {
                            iconSource: {
                                if(model&&model.icon){
                                    return model.icon
                                }
                                return 0
                            }
                            color: {
                                if(!item_control.enabled){
                                    return d.itemDisableColor
                                }
                                return "#000000"
                            }
                            iconSize: 15
                        }
                    }
                    Item { // 图标容器
                        id:item_icon
                        height: 30
                        width: visible ? 30 : 8
                        visible: {
                            if(model){
                                return model.iconVisible
                            }
                            return true
                        }
                        anchors{
                            left:parent.left
                            verticalCenter: parent.verticalCenter
                            leftMargin: 3
                        }
                        QuickLoader{
                            anchors.centerIn: parent
                            sourceComponent: {
                                if(model&&model.iconDelegate){
                                    return model.iconDelegate
                                }
                                return com_icon
                            }
                        }
                    }
                    QuickText { // 标题文字
                        id:item_title
                        text:{
                            if(model){
                                if(!item_icon.visible){
                                    return model.title[0]
                                }
                                return model.title
                            }
                            return ""
                        }
                        visible: true
                        elide: Text.ElideRight
                        color:{
                            if(!item_control.enabled){
                                return d.itemDisableColor
                            }
                            if(item_mouse.pressed){
                                return QuickColor.Grey120
                            }
                            return QuickColor.Grey220
                        }
                        anchors{
                            verticalCenter: parent.verticalCenter
                            left:item_icon.right
                            right: item_dot_loader.left
                        }
                    }
                    QuickLoader {
                        id:item_edit_loader
                        anchors{
                            top: parent.top
                            bottom: parent.bottom
                            left: item_title.left
                            right: item_title.right
                            rightMargin: 8
                        }
                        sourceComponent: {
                            if(d.isCompact){
                                return undefined
                            }
                            if(!model){
                                return undefined
                            }
                            return model.showEdit ? model.editDelegate : undefined
                        }
                        onStatusChanged: {
                            if(status === QuickLoader.Ready){
                                item.forceActiveFocus()
                                item_connection_edit_focus.target = item
                            }
                        }
                        Connections{
                            id:item_connection_edit_focus
                            ignoreUnknownSignals:true
                            function onActiveFocusChanged(focus){
                                if(focus === false){
                                    model.showEdit = false
                                }
                            }
                            function onCommit(text){
                                model.title = text
                                model.showEdit = false
                            }
                        }
                    }
                    QuickLoader {
                        id:item_dot_loader
                        property bool isDot: (item_dot_loader.item&&item_dot_loader.item.isDot)
                        anchors{
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                            rightMargin: isDot ? 3 : 10
                            verticalCenterOffset: isDot ? -8 : 0
                        }
                        sourceComponent: {
                            if(model&&model.infoBadge){
                                return model.infoBadge
                            }
                            return undefined
                        }
                    }
                }
            }
        }
    }

//    Item {
//        id:nav_app_bar
//        width: parent.width
//        height: visible ? 40 : 0
//        anchors{
//            top: parent.top
//            topMargin: control.topPadding
//        }
//        visible: !control.hideNavAppBar
//        z:999
//        RowLayout{
//            height:parent.height
//            spacing: 0
//            FluIconButton{
//                id:btn_back
//                iconSource: FluentIcons.ChromeBack
//                Layout.leftMargin: 5
//                Layout.alignment: Qt.AlignVCenter
//                disabled:  {
//                    return d.stackItems.length <= 1
//                }
//                iconSize: 13
//                onClicked: {
//                    d.stackItems = d.stackItems.slice(0, -1)
//                    var item = d.stackItems[d.stackItems.length-1]
//                    if(item._idx<(nav_list.count - layout_footer.count)){
//                        layout_footer.currentIndex = -1
//                    }else{
//                        layout_footer.currentIndex = item._idx-(nav_list.count-layout_footer.count)
//                    }
//                    nav_list.currentIndex = item._idx
//                    if(pageMode === FluNavigationViewType.Stack){
//                        var nav_stack = loader_content.item.navStack()
//                        var nav_stack2 = loader_content.item.navStack2()
//                        nav_stack.pop()
//                        if(nav_stack.currentItem.launchMode === FluPageType.SingleInstance){
//                            var url = nav_stack.currentItem.url
//                            var pageIndex = -1
//                            for(var i=0;i<nav_stack2.children.length;i++){
//                                var obj =  nav_stack2.children[i]
//                                if(obj.url === url){
//                                    pageIndex = i
//                                    break
//                                }
//                            }
//                            if(pageIndex !== -1){
//                                nav_stack2.currentIndex = pageIndex
//                            }
//                        }
//                    }else if(pageMode === FluNavigationViewType.NoStack){
//                        loader_content.setSource(item._ext.url,item._ext.argument)
//                    }
//                }
//            }
//            FluIconButton{
//                id:btn_menu
//                iconSource: FluentIcons.GlobalNavButton
//                iconSize: 15
//                Layout.preferredWidth: d.isMinimal ? 30 : 0
//                Layout.preferredHeight: 30
//                Layout.alignment: Qt.AlignVCenter
//                clip: true
//                onClicked: {
//                    d.enableNavigationPanel = !d.enableNavigationPanel
//                }
//                visible: opacity
//                opacity: d.isMinimal
//                Behavior on opacity{
//                    enabled: FluTheme.animationEnabled && d.animDisabled
//                    NumberAnimation{
//                        duration: 83
//                    }
//                }
//                Behavior on Layout.preferredWidth {
//                    enabled: FluTheme.animationEnabled && d.animDisabled
//                    NumberAnimation{
//                        duration: 167
//                        easing.type: Easing.OutCubic
//                    }
//                }
//            }
//            Image{
//                id:image_logo
//                Layout.preferredHeight: 20
//                Layout.preferredWidth: 20
//                source: control.logo
//                Layout.leftMargin: {
//                    if(btn_menu.visible){
//                        return 12
//                    }
//                    return 5
//                }
//                sourceSize: Qt.size(40,40)
//                Layout.alignment: Qt.AlignVCenter
//                MouseArea{
//                    anchors.fill: parent
//                    onClicked: {
//                        logoClicked()
//                    }
//                }
//            }
//            FluText{
//                Layout.alignment: Qt.AlignVCenter
//                text:control.title
//                Layout.leftMargin: 12
//                font: FluTextStyle.Body
//            }
//        }
//        Item{
//            anchors.right: parent.right
//            height: parent.height
//            width: {
//                if(loader_action.item){
//                    return loader_action.item.width
//                }
//                return 0
//            }
//            FluLoader{
//                id:loader_action
//                anchors.centerIn: parent
//                sourceComponent: actionItem
//            }
//        }
//    }

//    Component{
//        id:com_stack_content
//        Item{
//            StackView{
//                id:nav_stack
//                anchors.fill: parent
//                clip: true
//                visible: !nav_stack2.visible
//                popEnter : Transition{}
//                popExit : Transition {}
//                pushEnter: Transition {}
//                pushExit : Transition{}
//                replaceEnter : Transition{}
//                replaceExit : Transition{}
//            }
//            StackLayout{
//                id:nav_stack2
//                anchors.fill: nav_stack
//                clip: true
//                visible: {
//                    if(!nav_stack.currentItem){
//                        return false
//                    }
//                    return FluPageType.SingleInstance === nav_stack.currentItem.launchMode
//                }
//            }
//            function navStack(){
//                return nav_stack
//            }
//            function navStack2(){
//                return nav_stack2
//            }
//        }
//    }

    QuickLoader { // 导航项具体页面内容加载
        id:loader_content
        anchors{
            left: parent.left
            top: parent.top // nav_app_bar.bottom
            right: parent.right
            bottom: parent.bottom
            leftMargin: control.cellWidth
        }
        Behavior on anchors.leftMargin {
            enabled: true // FluTheme.animationEnabled && d.animDisabled
            NumberAnimation{
                duration: 167
                easing.type: Easing.OutCubic
            }
        }
        // sourceComponent: com_stack_content
    }
    // MouseArea{
    //     anchors.fill: parent
    //     visible: d.isMinimalAndPanel||d.isCompactAndPanel
    //     hoverEnabled: true
    //     onWheel: {
    //     }
    //     onClicked: {
    //         d.enableNavigationPanel = false
    //     }
    // }

    Rectangle { // 导航页面布局
        id:layout_list
        width: control.cellWidth
        anchors {
            top: parent.top
            topMargin: control.navTopMargin
            bottom: parent.bottom
        }
        border.color: Qt.rgba(226/255,230/255,234/255,1)
        border.width: 0
        color: "transparent"
        QuickShadow{
            visible: false // d.isMinimal || d.isCompactAndPanel
            radius: 0
        }
        x: visible ? 0 : -width
        Behavior on width {
            enabled: true // FluTheme.animationEnabled && d.animDisabled
            NumberAnimation{
                duration: 167
                easing.type: Easing.OutCubic
            }
        }
        Behavior on x {
            enabled: true // FluTheme.animationEnabled && d.animDisabled
            NumberAnimation{
                duration: 167
                easing.type: Easing.OutCubic
            }
        }
        visible: true

        Item { // header 组件 (搜索栏)
            id:layout_header
            width: layout_list.width
            clip: true
            y: control.topPadding // nav_app_bar.height+control.topPadding
            height: autoSuggestBox ? 38 : 0
            QuickLoader {
                id:loader_auto_suggest_box
                sourceComponent: autoSuggestBox
                anchors{
                    left: parent.left
                    right: parent.right
                    leftMargin: 6
                    rightMargin: 6
                    verticalCenter: parent.verticalCenter
                }
                visible: true
            }
            QuickTextIconButton{
                visible: false
                anchors{
                    fill: parent
                    leftMargin: 6
                    rightMargin: 6
                    topMargin: 2
                    bottomMargin: 2
                }
                iconSize: 15
                iconSource: {
                    if(loader_auto_suggest_box.item){
                        return loader_auto_suggest_box.item.autoSuggestBoxReplacement
                    }
                    return 0
                }
                onClicked: {
                    d.enableNavigationPanel = !d.enableNavigationPanel
                }
            }
        }
        Flickable { // content
            id:layout_flickable
            anchors{
                top: layout_header.bottom
                topMargin: 6
                left: parent.left
                right: parent.right
                bottom: parent.bottom // layout_footer.top
            }
            boundsBehavior: ListView.StopAtBounds
            clip: true
            contentHeight: nav_list.contentHeight
            ScrollBar.vertical: QuickScrollBar {}
            ListView { // 导航项展示列表
                id:nav_list
                displaced: Transition {
                    NumberAnimation {
                        properties: "x,y"
                        easing.type: Easing.OutQuad
                    }
                }
                anchors.fill: parent
                interactive: false
                model:d.handleItems()
                boundsBehavior: ListView.StopAtBounds
                highlightMoveDuration: 167
                highlight: Item{
                    clip: true
                    Rectangle{
                        height: 18
                        radius: 1.5
                        color: QuickColor.Primary
                        width: 3
                        anchors{
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            leftMargin: 6
                        }
                    }
                }
                currentIndex: -1
                delegate: QuickLoader{
                    property var model: modelData
                    property var _idx: index
                    property int type: 0
                    sourceComponent: {
                        if (model === null || !model) {
                            return undefined
                        } else if (modelData instanceof QuickPaneItem) {
                            return com_panel_item
                        } else if (modelData instanceof QuickPaneItemHeader) {
                            return com_panel_item_header
                        } else if (modelData instanceof QuickPaneItemSeparator) {
                            return com_panel_item_separator
                        } else if (modelData instanceof QuickPaneItemExpander) {
                            return com_panel_item_expander
                        } else if (modelData instanceof QuickPaneItemEmpty) {
                            return com_panel_item_empty
                        }
                        return undefined
                    }
                }
            }
        }

        ListView { // footer
            id:layout_footer
            clip: true
            width: layout_list.width
            height: childrenRect.height
            anchors.bottom: parent.bottom
            interactive: false
            boundsBehavior: ListView.StopAtBounds
            currentIndex: -1
            model: d.handleFooterItems()
            highlightMoveDuration: 150
            highlight: Item{
                clip: true
                Rectangle{
                    height: 18
                    radius: 1.5
                    color: QuickColor.Primary
                    width: 3
                    anchors{
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 6
                    }
                }
            }
            delegate: QuickLoader {
                property var model: modelData
                property var _idx: index
                property int type: 1
                sourceComponent: {
                    if(modelData instanceof QuickPaneItem){
                        return com_panel_item
                    } else if(modelData instanceof QuickPaneItemHeader){
                        return com_panel_item_header
                    } else if(modelData instanceof QuickPaneItemSeparator){
                        return com_panel_item_separatorr
                    }
                }
            }
        }
    }
    Popup{ // 展开列表的子项显示
        property var childModel
        id: control_popup
        enter: Transition {
            NumberAnimation {
                property: "opacity"
                from:0
                to:1
                duration: 83
            }
        }
        // Connections{
        //     target: d
        //     function onIsCompactChanged(){
        //         if(!d.isCompact){
        //             control_popup.close()
        //         }
        //     }
        // }
        padding: 0
        focus: true
        contentItem: Item{
            ListView{
                id:list_view
                anchors.fill: parent
                clip: true
                currentIndex: -1
                model: control_popup.childModel
                boundsBehavior: ListView.StopAtBounds
                ScrollBar.vertical: QuickScrollBar {}
                delegate:Button{
                    id:item_button
                    width: 180
                    height: 38
                    focusPolicy:Qt.TabFocus
                    background: Rectangle{
                        color:  {
                            if(item_button.hovered){
                                return QuickColor.ItemHover
                            }
                            return QuickColor.ItemNormal
                        }
                        QuickFocusRectangle{
                            visible: item_button.activeFocus
                            radius:4
                        }

                        QuickLoader{
                            id:item_dot_loader
                            anchors{
                                right: parent.right
                                verticalCenter: parent.verticalCenter
                                rightMargin: 10
                            }
                            sourceComponent: {
                                if(model.infoBadge){
                                    return model.infoBadge
                                }
                                return undefined
                            }
                        }

                    }
                    contentItem: QuickText{
                        text:modelData.title
                        elide: Text.ElideRight
                        rightPadding: item_dot_loader.width
                        verticalAlignment: Qt.AlignVCenter
                        anchors{
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    onClicked: {
                        if(modelData.onTapListener){
                            modelData.onTapListener()
                        }else{
                            modelData.tap()
                            nav_list.currentIndex = _idx
                            layout_footer.currentIndex = -1
                        }
                        control_popup.close()
                    }
                }
            }
        }
        background: Rectangle{
            implicitWidth: 180
            radius: [4,4,4,4]
            QuickShadow{
                radius: 4
            }
            color: Qt.rgba(248/255,250/255,253/255,1)
        }
        function showPopup(pos,height,model){
            background.implicitHeight = height
            control_popup.x = pos.x
            control_popup.y = pos.y
            control_popup.childModel = model
            control_popup.open()
        }
    }
    QuickLoader{
        property var modelData
        id:loader_item_menu
    }
    Connections{
        id:connection_item_menu
        function onVisibleChanged(visible){
            if(target.visible === false){
                loader_item_menu.sourceComponent = undefined
            }
        }
    }
    Component{
        id:com_placeholder
        Item{
            property int launchMode: FluPageType.SingleInstance
            property string url
        }
    }
    function collapseAll(){
        for(var i=0;i<nav_list.model.length;i++){
            var item = nav_list.model[i]
            if(item instanceof QuickPaneItemExpander){
                item.isExpand = false
            }
        }
    }
    function setCurrentIndex(index){
        var item = nav_list.model[index]
        if(item.url){
            nav_list.currentIndex = index
            if(item instanceof QuickPaneItem){
                item.tap()
            }
        }else{
            item.onTapListener()
        }
    }
    function getItems(){
        return nav_list.model
    }
    function getCurrentIndex(){
        return nav_list.currentIndex
    }
    function getCurrentUrl(){
        return loader_content.source.toString()
    }

    function push(url,argument={}){
        function noStackPush(){
            if(loader_content.source.toString() === url){
                return
            }
            loader_content.setSource(url,argument)
            var obj = nav_list.model[nav_list.currentIndex]
            obj._ext = {url:url,argument:argument}
        }
        noStackPush()
    }
    function startPageByItem(data){
        var items = getItems()
        for(var i=0;i<items.length;i++){
            var item =  items[i]
            if(item.group && data.group && item.group === data.group){
                if ((item.task || data.task) && item.task !== data.task) {
                    continue
                }
                if(getCurrentIndex() === i){
                    return
                }
                setCurrentIndex(i)
                if(item._parent){
                    item._parent.isExpand = true
                }
                return
            }
        }
    }
}
