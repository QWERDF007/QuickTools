import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: scaleableImage
    clip: true
    width: 200
    height: 200
    // implicitHeight: _image.implicitHeight // 不确定绑定Image的隐式宽高有没有问题
    // implicitWidth: _image.implicitWidth

    property alias image: _image
    property alias status: _image.status
    property alias source: _image.source
    property alias sourceSize: _image.sourceSize
    property bool imageDragEnable: false
    property real stepSize: {
        if (_image.scale < 2) {
            return 0.1
        } else if (_image.scale < 10) {
            return 1
        } else {
            return 2
        }
    }

    property real from: 0.25
    property real to: 32
    property var scaledImagePos: mapFromItem(_image, 0, 0)
    property bool isFitInView: true
    property real imageSourceScale: {
        if (_image.source !== "" && _image.status === _image.Ready) {
            return Math.min(scaleableImage.height / _image.sourceSize.height, scaleableImage.width / _image.sourceSize.width)
        }
        return 1.0
    }

    Image {
        id: _image
        // asynchronous: true // 异步加载会导致自适应窗口出问题
        property real xOffset: Math.abs(width - paintedWidth) / 2 * scale
        property real yOffset: Math.abs(height - paintedHeight) / 2 * scale
        fillMode: Image.PreserveAspectFit
        onXChanged: {
            updateImagePos()
        }
        onYChanged: {
            updateImagePos()
        }
        onStatusChanged: {
            if (_image.status === Image.Ready) {
                fitInView()
            }
        }
        transformOrigin: Item.TopLeft
    }

    onWidthChanged: {
        if (isFitInView) {
            fitInView()
        }
    }
    onHeightChanged: {
        if (isFitInView) {
            fitInView()
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: scaleableImage.imageDragEnable ? _image : null
        drag.axis: Drag.XAndYAxis
        hoverEnabled: true
        acceptedButtons: Qt.AllButtons

        onPressed: function (mouse) {
            scaleableImage.forceActiveFocus()
            if (mouse.button === Qt.LeftButton) {
                if (mouse.modifiers & Qt.ControlModifier) {
                    setImageDragEnable(true)
                    setCursorShape(Qt.ClosedHandCursor)
                }
            } else if (mouse.button === Qt.MiddleButton) {
                setImageDragEnable(true)
                setCursorShape(Qt.ClosedHandCursor)
            }
        }

        onReleased: function (mouse) {
            if (scaleableImage.imageDragEnable) {
                setImageDragEnable(false)
                if (mouse.modifiers & Qt.ControlModifier) {
                    setCursorShape(Qt.OpenHandCursor)
                } else {
                    setCursorShape(Qt.ArrowCursor)
                }
            }
        }

        onPositionChanged: function (mouse) {
        }

        onWheel: function (wheel) {
            scaleImageByWheel(wheel)
            updateImagePos()
        }
    }

    Keys.onPressed: function(event) {
        if (event.key === Qt.Key_Control) {
            if (mouseArea.containsPress) {
                setCursorShape(Qt.ClosedHandCursor)
            } else {
                setCursorShape(Qt.OpenHandCursor)
            }
        } else if (event.key === Qt.Key_Space) {
            fitInView()
        }
    }

    Keys.onReleased: function(event) {
        if (event.key === Qt.Key_Control && !mouseArea.containsPress) {
            setCursorShape(Qt.ArrowCursor)
        }
    }

    /**
     * @brief 设置图片是否可拖拽
     * @param enable
     */
    function setImageDragEnable(enable) {
        scaleableImage.imageDragEnable = enable
    }

    /**
     * @brief 在中心缩放图像
     * @param scale
     */
    function scaleInCenter(scale) {
        var x = scaleableImage.width / 2
        var y = scaleableImage.height / 2
        // 将图像剧中
        _image.x = x - _image.paintedWidth / 2
        _image.y = y - _image.paintedHeight / 2
        var scaleOrigin = mapToItem(_image, x, y)
        // _image.scale = Math.min(Math.max(from * scaleableImage.imageSourceScale, scale), to * scaleableImage.imageSourceScale)
        _image.scale = Math.min(Math.max(from, scale), to)
        // 鼠标位置相对于缩放后图像的位置
        var pos = mapFromItem(_image, scaleOrigin)
        //按照差值移动一下图，使得图看起来在鼠标位置缩放
        _image.x -= pos.x - x
        _image.y -= pos.y - y
    }

    /**
     * @brief 鼠标滚轮缩放图片, 更新图片位置
     * @param wheel
     */
    function scaleImageByWheel(wheel) {
        // 鼠标相对于缩放前图像的位置
        var scaleOrigin = mapToItem(_image, wheel.x, wheel.y)
        // 缩放
        var step = wheel.angleDelta.y / 120 * scaleableImage.stepSize
        // _image.scale = Math.min(Math.max(from * scaleableImage.imageSourceScale, _image.scale + step), to * scaleableImage.imageSourceScale)
        _image.scale = Math.min(Math.max(from, _image.scale + step), to)
        // 鼠标位置相对于缩放后图像的位置
        var pos = mapFromItem(_image, scaleOrigin)
        //按照差值移动一下图，使得图看起来在鼠标位置缩放
        _image.x -= pos.x - wheel.x
        _image.y -= pos.y - wheel.y
    }

    /**
     * @brief 更新缩放后的图像起始点
     */
    function updateImagePos() {
        scaledImagePos = mapFromItem(_image, 0, 0)
    }

    /**
     * @brief 设置图片区域的鼠标形状
     * @param cursorShape
     */
    function setCursorShape(cursorShape) {
        mouseArea.cursorShape = cursorShape
    }

    /**
     * @brief 图像适应窗口
     */
    function fitInView() {
        if (!scaleableImage.isFitInView || _image.sourceSize.height === 0 || _image.sourceSize.width === 0)
            return
        scaleableImage.imageSourceScale = Math.min(scaleableImage.height / _image.sourceSize.height, scaleableImage.width / _image.sourceSize.width)
        // 缩放后的原点
        var scaleOrigin = mapToItem(_image, 0, 0)
        _image.scale = scaleableImage.imageSourceScale
        var dx = (scaleableImage.width - _image.sourceSize.width * _image.scale) / 2
        var dy = (scaleableImage.height - _image.sourceSize.height * _image.scale) / 2
        var pos = mapFromItem(_image, scaleOrigin)
        // 按照差值移动一下图，使得图看起来在(0,0)处缩放
        _image.x -= pos.x
        _image.y -= pos.y
        // 移动到窗口中央
        _image.x -= scaledImagePos.x - dx
        _image.y -= scaledImagePos.y - dy
    }
}
