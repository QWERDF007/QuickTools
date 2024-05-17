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
        ToolButton {
            icon.source: "/icons/image"
            implicitHeight: Math.min(parent.width, parent.height)
            implicitWidth: Math.min(parent.width, parent.height)

            onClicked: {
                fileDialog.open();
            }
        }
    }
    FileDialog {
        id: fileDialog

        folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
        nameFilters: ["Image files (*.jpg *.jpeg *.png *.bmp)"]

        onAccepted: {
            var path = fileDialog.file.toString().toLowerCase().slice(8);
            valueChanged(path);
        }
    }
}
