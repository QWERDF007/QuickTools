import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: control
    property QuickObject items
    property Component autoSuggestBox

    property int navTopMargin: 0
    property int cellHeight: 38
    property int cellWidth: 300

    Item {
        id:d
        property bool isCompact: false
        property bool isMinimal: false

        property color itemDisableColor: Qt.rgba(160/255,160/255,160/255,1)

        function handleItems(){
            var _idx = 0
            var data = []
            if(items){
                for(var i=0;i<items.children.length;i++){
                    var item = items.children[i]
                    item._idx = _idx
                    data.push(item)
                    _idx++
                    if(item instanceof QuickPaneItemExpander){
                        for(var j=0;j<item.children.length;j++){
                            var itemChild = item.children[j]
                            itemChild._parent = item
                            itemChild._idx = _idx
                            data.push(itemChild)
                            _idx++
                        }
                    }
                }
//                if(footerItems){
//                    var comEmpty = Qt.createComponent("FluPaneItemEmpty.qml");
//                    for(var k=0;k<footerItems.children.length;k++){
//                        var itemFooter = footerItems.children[k]
//                        if (comEmpty.status === Component.Ready) {
//                            var objEmpty = comEmpty.createObject(items,{_idx:_idx});
//                            itemFooter._idx = _idx;
//                            data.push(objEmpty)
//                            _idx++
//                        }
//                    }
//                }
            }
            return data
        }
    }


    // 分割线
    Component{
        id:com_panel_item_separatorr
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

    // 展开列表
    Component{
        id:com_panel_item_expander
        Item{
            height: control.cellHeight
            width: layout_list.width
            QuickControl{
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
//                FluTooltip {
//                    text: model.title
//                    visible: item_control.hovered && model.title && d.isCompact
//                    delay: 800
//                }
                MouseArea{
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
                onClicked: {
                    if(d.isCompactAndNotPanel && model.children.length > 0){
                        let h = 38*Math.min(Math.max(model.children.length,1),8)
                        let y = mapToItem(control,0,0).y
                        if(h+y>control.height){
                            y = control.height - h
                        }
                        control_popup.showPopup(Qt.point(control.navCompactWidth,y),h,model.children)
                        return
                    }
                    model.isExpand = !model.isExpand
                }
                Rectangle{
                    color: Qt.rgba(255/255,77/255,79/255,1)
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
                Rectangle {
                    radius: 4
                    anchors.fill: parent
                    Rectangle{
                        width: 3
                        height: 18
                        radius: 1.5
//                        color: FluTheme.primaryColor
                        visible: {
                            if(!model){
                                return false
                            }
                            if(!model.children){
                                return false
                            }
                            for(var i=0;i<model.children.length;i++){
                                var item = model.children[i]
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
                    QuickIcon {
                        id:item_icon_expand
                        rotation: model&&model.isExpand?0:180
//                        iconSource: FluentIcons.ChevronUp
                        iconSize: 15
                        anchors{
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                            rightMargin: visible ? 12 : 0
                        }
                        visible: true
                        Behavior on rotation {
//                            enabled: FluTheme.enableAnimation && d.animDisabled
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
                    color: {
                        if(!item_control.enabled){
                            return Qt.rgba(0,0,0,0)
                        }
                        if(nav_list.currentIndex === _idx&&type===0){
                            return Qt.rgba(0,0,0,255*0.09)
                        }
                        if(item_control.hovered){
                            return Qt.rgba(0,0,0,255*0.03)
                        }
                        return Qt.rgba(0,0,0,0)
                    }
                    Component{
                        id:com_icon
                        QuickIcon{
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
                    Item{
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
//                    FluText{
//                        id:item_title
//                        text:{
//                            if(model){
//                                if(!item_icon.visible && d.isCompactAndNotPanel){
//                                    return model.title[0]
//                                }
//                                return model.title
//                            }
//                            return ""
//                        }
//                        visible: {
//                            if(d.isCompactAndNotPanel){
//                                if(item_icon.visible){
//                                    return false
//                                }
//                                return true
//                            }
//                            return true
//                        }
//                        elide: Text.ElideRight
//                        anchors{
//                            verticalCenter: parent.verticalCenter
//                            left:item_icon.right
//                            right: item_icon_expand.left
//                        }
//                        color:{
//                            if(!item_control.enabled){
//                                return d.itemDisableColor
//                            }
//                            if(item_control.pressed){
//                                return FluTheme.dark ? FluColors.Grey80 : FluColors.Grey120
//                            }
//                            return FluTheme.dark ? FluColors.White : FluColors.Grey220
//                        }
//                    }

                    Text {
                        id:item_title
                        text: model.title
                        anchors{
                            verticalCenter: parent.verticalCenter
                            left:item_icon.right
                            right: item_icon_expand.left
                        }
                        Component.onCompleted: {
                            console.log("text", text, width, height)
                        }
                    }

                    QuickLoader{
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
                            ignoreUnknownSignals: true
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


    Rectangle{
        id:layout_list
        width: control.cellWidth
        anchors {
            top: parent.top
            topMargin: control.navTopMargin
            bottom: parent.bottom
        }

        color: "transparent"

        Item{
            id:layout_header
            width: layout_list.width
            clip: true
        }

        Flickable{
            id:layout_flickable
            anchors{
                top: layout_header.bottom
                topMargin: 6
                left: parent.left
                right: parent.right
//                bottom: layout_footer.top
                bottom: parent.top
            }
            boundsBehavior: ListView.StopAtBounds
            clip: true
            contentHeight: nav_list.contentHeight
//            ScrollBar.vertical: FluScrollBar {}
            ListView{
                id:nav_list
                Component.onCompleted: {
                    console.log("lv", width, height)
                }

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
                        color: FluTheme.primaryColor
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
                        if(model === null || !model)
                            return undefined
                        if(modelData instanceof QuickPaneItem){
                            return com_panel_item
                        }
                        if(modelData instanceof QuickPaneItemHeader){
                            return com_panel_item_header
                        }
                        if(modelData instanceof QuickPaneItemSeparator){
                            return com_panel_item_separatorr
                        }
                        if(modelData instanceof QuickPaneItemExpander){
                            return com_panel_item_expander
                        }
                        if(modelData instanceof QuickPaneItemEmpty){
                            return com_panel_item_empty
                        }
                        return undefined
                    }
                }
            }
        }

    }
}
