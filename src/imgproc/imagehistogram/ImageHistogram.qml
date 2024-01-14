import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.core
import QuickTools.ui

Window {
    id: imageHistogramWin
    width: 1080
    height: 720
    visible: true
    color: active ? QuickColor.WindowActiveBackground : QuickColor.WindowBackground

    property QuickTool quicktool: QuickToolFactor.createQuickTool(QuickToolType.ImageHistogram)
    property QuickInputParam inputParam: quicktool.inputParams
    property string imagePath: {
        if (quicktool == null || inputParam == null) {
            return null
        }
        console.log(objToString(inputParam))
    }

    signal start

    Component.onCompleted: {
        console.log(quicktool.name)
    }

    onStart: function() {
        quicktool.run()
    }

    /**
     * @brief 获取 obj 的属性并转换为字符串, 包括 function 和 property
     * @param obj
     */
    function objToString (obj) {
        var str = '';
        for (var p in obj) {
            if (Object.prototype.hasOwnProperty.call(obj, p)) {
                str += p + '::' + obj[p] + '\n';
            }
        }
        return str;
    }

    Item {
        anchors.fill: parent
        ListView {
            id: paramsView
            width: image.width
            implicitHeight: 32
            orientation: Qt.Horizontal
            spacing: 5
            model: inputParam

            delegate: Item {
                width: model.visible ? 40 : 0
                height: paramsView.height
                visible: model.visible
                Rectangle {
                    width: 40
                    height: paramsView.height
                    color: "lightgray"
                    Text {
                        text: model.index
                    }
                }
            }
        }

        SplitView {
            id: sv1
            anchors {
                left: parent.left
                right: parent.right
                top: paramsView.bottom
                bottom: parent.bottom
            }

            orientation: Qt.Horizontal
            handle:  Rectangle {
                implicitWidth: 4
                implicitHeight: sv1.height
                color: Qt.darker(Qt.rgba(226/255,229/255,234/255,1), 1.1)

                Rectangle {
                    implicitWidth: 2
                    implicitHeight: 12
                    anchors.centerIn: parent
                }
            }
            QuickArea {
                id: imageContainer
                SplitView.fillHeight: true
                SplitView.fillWidth: true
                SplitView.minimumWidth: 160
                clip: true

                Image {
                    id: image
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    onSourceChanged: {
                        imageHistogramWin.start()
                    }
                }

                Item {
                    visible: image.status === Image.Null
                    implicitHeight: childrenRect.height
                    anchors.centerIn: parent
                        Image {
                            id: dropimage
                            width: 80
                            height: 80
                            anchors {
                                top: parent.top
                                horizontalCenter: parent.horizontalCenter
                            }
                            source: "/icons/dropimage"
                        }
                        QuickText {
                            id: droptip
                            anchors {
                                top: dropimage.bottom
                                topMargin: 5
                                horizontalCenter: parent.horizontalCenter
                            }

                            text: "DROP YOUR IMAGES HERE"
                        }
                        QuickText {
                            id: seperator
                            anchors {
                                top: droptip.bottom
                                topMargin: 5
                                horizontalCenter: parent.horizontalCenter
                            }
                            text: "- OR -"
                        }

                        QuickButton {
                            id: browseBtn
                            anchors {
                                top: seperator.bottom
                                topMargin: 5
                                horizontalCenter: parent.horizontalCenter
                            }
                            text: "BROWSE FROM YOUR COMPUTER"
                        }
                }

                DropArea {
                    anchors.fill: parent
                    onDropped: function(drop) {
                        console.log("drop", drop.urls)
                        var url = drop.urls[0]
                        if (isImageFile(url) && image.source !== url) {
                            image.source = url
                        }
                    }
                    function isImageFile(url) {
                        var path = url.toString().toLowerCase()
                        return path.startsWith("file:") && (path.endsWith(".jpg") || path.endsWith(".png") || path.endWith(".jpeg"))
                    }
                }
            }
            Item {
                id: histogramContainer
                implicitWidth: 200
            }
        }
    }
}