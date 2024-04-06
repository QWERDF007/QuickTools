import QtQuick
import QtQuick.Controls
import Qt.labs.platform
import QuickTools.ui

Item {
    id: dropImageArea
    property bool dropBtnAreaVisible: true
    property string imagePath: ""
    signal pathChanged(string path)
    Item {
        id: dropBtnArea
        visible: dropBtnAreaVisible
        implicitHeight: childrenRect.height
//        implicitWidth: childrenRect.width
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

            text: qsTr("拖放图像到此")
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
            text: qsTr("浏览图像文件")
            onClicked: {
                fileDialog.open()
            }
        }

        FileDialog {
            id: fileDialog
            folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
            nameFilters: ["Image files (*.jpg *.jpeg *.png *.bmp)"]
            onAccepted: {
                var url = fileDialog.file
                if (!isImageFile(url)) {
                    return
                }
                var path = getImagePath(url)
                if (path !== dropImageArea.imagePath) {
                    pathChanged(path)
                }
            }
        }
    }
    DropArea {
        anchors.fill: parent
        onDropped: function(drop) {
            var url = drop.urls[0]
            if (!isImageFile(url)) {
                return
            }
            var path = getImagePath(url)
            if (path !== dropImageArea.imagePath) {
                pathChanged(path)
            }
        }
    }

    function getImagePath(url) {
        return url.toString().toLowerCase().slice(8)
    }

    function isImageFile(url) {
        var path = url.toString().toLowerCase()
        return path.startsWith("file:") && (path.endsWith(".jpg") || path.endsWith(".png") || path.endsWith(".jpeg"))
    }
}
