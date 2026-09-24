import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import quickui
import QuickTools.ui

T_SettingItem {
    id: fileFolderInputSettingItem
    RowLayout {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        TextField {
            id: _content
            Layout.minimumWidth: 320
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: settingValue !== undefined && settingValue !== null ? String(settingValue) : ""
            onEditingFinished: {
                valueChanged(text)
            }
        }

        QuiToolButton {
            id: folderBtn
            icon.source: "/icons/folder"
            implicitHeight: 32
            implicitWidth: 32

            onClicked: {
                if (settingAdditional && settingAdditional.isFile) {
                    fileDialog.open()
                } else {
                    folderDialog.open()
                }
            }
            QuiToolTip{
                text: qsTr("选择文件或目录")
                visible: folderBtn.hovered
                delay: 200
            }
        }
    }
    FolderDialog {
        id: folderDialog
        onAccepted: {
            var folder = folderDialog.folder.toString().slice(8)
            valueChanged(folder)
        }
    }

    FileDialog {
        id: fileDialog

        folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
        nameFilters: settingAdditional && settingAdditional.filter ? [settingAdditional.filter] : ["*"]
        onAccepted: {
            var path = fileDialog.file.toString().slice(8)
            valueChanged(path)
        }
    }
}
