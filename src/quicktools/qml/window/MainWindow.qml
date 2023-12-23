import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QuickTools.ui

import "../global"

Window {
    id: root
    visible: true
    width: 1920
    height: 1080
    color: active ? QuickColor.WindowActiveBackground : QuickColor.WindowBackground

    Flipable {
        id:flipable
        anchors.fill: parent
        property bool flipped: false
        property real flipAngle: 0
        transform: Rotation {
            id: rotation
            origin.x: flipable.width/2
            origin.y: flipable.height/2
            axis { x: 0; y: 1; z: 0 }
            angle: flipable.flipAngle

        }
        states: State {
            PropertyChanges { target: flipable; flipAngle: 180 }
            when: flipable.flipped
        }
        transitions: Transition {
            NumberAnimation { target: flipable; property: "flipAngle"; duration: 1000 ; easing.type: Easing.OutCubic}
        }

        back: Item {
            anchors.fill: flipable
            visible: flipable.flipAngle !== 0
        }
        front: Item {
            visible: flipable.flipAngle !== 180
            anchors.fill: flipable

            QuickNavigationView {
                id: nav_view
                anchors.fill: parent
                items: QuickToolsItems
                Component.onCompleted: {
                    QuickToolsItems.navigationView = nav_view
                    setCurrentIndex(0)
                }
            }
        }
    }
}
