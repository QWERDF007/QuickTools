import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.platform
import QuickTools.ui
import QuickTools.core

T_ParamItem {
    id: inputImageItem

    displayText: _content.text
    tooltipText: _content.text
    tooltipVisible: _content.truncated

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 0
        spacing: 2

        QuickText {
            id: _content

            Layout.fillHeight: true
            Layout.fillWidth: true
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            text: paramDisplay
            verticalAlignment: Text.AlignVCenter
        }
        QuickToolButton {
            id: imageBtn
            icon.source: "/icons/image"
            implicitHeight: Math.min(parent.width, parent.height)
            implicitWidth: Math.min(parent.width, parent.height)

            onClicked: {
                if (paramDisplay)
                    fileDialog.folder = "file:///" + UITools.getDirectory(paramDisplay)
                fileDialog.open()
            }
            QuickToolTip{
                text: qsTr("打开图像")
                visible: imageBtn.hovered
                delay: 200
            }
        }
        QuickToolButton {
            id: folderBtn
            icon.source: "/icons/folder"
            implicitHeight: Math.min(parent.width, parent.height)
            implicitWidth: Math.min(parent.width, parent.height)

            onClicked: {
                if (paramDisplay)
                    fileDialog.folder = "file:///" + UITools.getDirectory(paramDisplay)
                folderDialog.open()
            }
            QuickToolTip{
                text: qsTr("打开文件夹")
                visible: folderBtn.hovered
                delay: 200
            }
        }
    }

    FileDialog {
        id: fileDialog

        folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
        nameFilters: ["Image files (*.jpg *.jpeg *.png *.bmp *.webp)"]

        onAccepted: {
            var path = fileDialog.file.toString().toLowerCase().slice(8);
            valueChanged(path);
        }
    }

    FolderDialog  {
        id: folderDialog

        folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)

        onAccepted: {
            var path = folderDialog.folder.toString().toLowerCase().slice(8)
            valueChanged(path)
        }
    }
}
