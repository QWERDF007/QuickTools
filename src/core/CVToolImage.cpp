#include "core/CVToolImage.h"

#include <opencv2/imgproc.hpp>

namespace quicktools::core {

QImage mat2QImage(const cv::Mat &mat)
{
    QImage image;
    switch (mat.type())
    {
    case CV_8UC1:
        // QImage构造：数据，宽度，高度，每行多少字节，存储结构
        image = QImage((const unsigned char *)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        break;
    case CV_8UC3:
        image = QImage((const unsigned char *)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        image = image.rgbSwapped(); // BRG转为RGB
        // Qt5.14增加了Format_BGR888
        // image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.cols * 3, QImage::Format_BGR888);
        break;
    case CV_8UC4:
        image = QImage((const unsigned char *)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        break;
    case CV_16UC4:
        image = QImage((const unsigned char *)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGBA64);
        image = image.rgbSwapped(); // BRG转为RGB
        break;
    }
    return image;
}

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
    qInfo() << __FUNCTION__ << __LINE__ << this;
}

ImageProvider::~ImageProvider()
{
    qInfo() << __FUNCTION__ << __LINE__ << this;
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qInfo() << __FUNCTION__ << __LINE__ << id;
    if (size)
        *size = QSize(image_.cols, image_.rows);
    cv::Mat dst;
    if (requestedSize.isValid())
        cv::resize(image_, dst, cv::Size(requestedSize.width(), requestedSize.height()), 0, 0, cv::INTER_CUBIC);
    else
        dst = image_;
    qInfo() << __FUNCTION__ << __LINE__ << dst.rows << dst.cols << dst.channels();
    return mat2QImage(dst);
}

void ImageProvider::setImage(const cv::Mat &image)
{
    image_ = image;
    qInfo() << __FUNCTION__ << __LINE__ << image_.rows << image_.cols << image_.channels();
}

CVToolImage::CVToolImage(const QString &name, const QString& uuid, QObject *parent, QQmlEngine * qml_engine, QJSEngine* js_engine)
    : QObject(parent)
    , name_(name)
    , uuid_(uuid)
    , provider_(new ImageProvider())
    , qml_engine_(qml_engine)
    , js_engine_(js_engine)
{
    qInfo() << __FUNCTION__ << __LINE__ << this;
    if (qml_engine_ && provider_)
    {
        qInfo() << __FUNCTION__ << __LINE__ << "addImageProvider" << getUrl();
        qml_engine_->addImageProvider(getUrl(), provider_);
    }

}

CVToolImage::~CVToolImage()
{
    qInfo() << __FUNCTION__ << __LINE__ << this;
    if (qml_engine_ && provider_)
    {
        qInfo() << __FUNCTION__ << __LINE__ << "removeImageProvider" << getUrl();
        qml_engine_->removeImageProvider(getUrl());
    }

}

void CVToolImage::setImage(const QString &path, const cv::Mat &image)
{
    if (provider_)
        provider_->setImage(image);
    if (path_ != path)
    {
        emit urlChanged();
        emit pathChanged();
    }
    else
    {
        emit imageChanged();
    }
}

QString CVToolImage::getUrl() const
{
    return QString("image://%1/%2/id").arg(name_, uuid_);
}


QString CVToolImage::path() const
{
    return path_;
}

bool CVToolImage::setPath(const QString &path)
{
    if (path == path_)
        return false;
    path_ = path;
    emit pathChanged();
    return true;
}

} // namespace quicktools::core
