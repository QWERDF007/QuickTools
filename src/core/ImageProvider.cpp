#include "core/ImageProvider.h"

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

ImageProvider::ImageProvider(const QString &name, const QString &uuid)
    : QQuickImageProvider(QQuickImageProvider::Image)
    , name_(name)
    , uuid_(uuid)
{
}

ImageProvider::~ImageProvider() {}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id)
    if (size)
        *size = QSize(image_.cols, image_.rows);
    cv::Mat dst;
    if (requestedSize.isValid())
        cv::resize(image_, dst, cv::Size(requestedSize.width(), requestedSize.height()), 0, 0, cv::INTER_CUBIC);
    else
        dst = image_;
    return mat2QImage(dst);
}

void ImageProvider::setImage(const QString &path, const cv::Mat &image)
{
    if (path_ == path)
        return;
    path_  = path;
    image_ = image;
    emit imageChanged();
}

QString ImageProvider::id() const
{
    return QString("%1-%2").arg(name_, uuid_);
}

QString ImageProvider::url() const
{
    return QString("image://%1/this is id").arg(id());
}

ImageProviderWrapper::ImageProviderWrapper(const QString &name, const QString &uuid, QObject *parent,
                                           QQmlEngine *qml_engine, QJSEngine *js_engine)
    : QObject(parent)
    , qml_engine_(qml_engine)
    , js_engine_(js_engine)
    , image_provider_(new ImageProvider(name, uuid))
{
    if (qml_engine_ && image_provider_)
    {
        qml_engine_->addImageProvider(image_provider_->id(), image_provider_);
        connect(image_provider_, &ImageProvider::imageChanged, this, &ImageProviderWrapper::imageChanged,
                Qt::DirectConnection);
    }
}

ImageProviderWrapper::~ImageProviderWrapper()
{
    if (qml_engine_ && image_provider_)
        qml_engine_->removeImageProvider(image_provider_->id());
}

ImageProviderList::ImageProviderList(QObject *parent, QQmlEngine *qml_engine, QJSEngine *js_engine)
    : QObject(parent)
    , qml_engine_(qml_engine)
    , js_engine_(js_engine)
{
}

ImageProviderList::~ImageProviderList()
{
    for (const ImageProvider *provider : providers_)
    {
        if (qml_engine_ && provider)
        {
            qml_engine_->removeImageProvider(
                provider->id()); // 此处会自动 delete provider, 不能再手动 delete, 否则会 double delete
            // delete provider;
            // provider = nullptr;
        }
    }
}

int ImageProviderList::size() const
{
    return providers_.size();
}

QList<ImageProvider *> ImageProviderList::providers() const
{
    return providers_;
}

bool ImageProviderList::addImageProvider(const QString &name, const QString &uuid)
{
    for (const ImageProvider *provider : providers_)
    {
        QString id = QString("%1-%2").arg(name, uuid);
        if (provider && provider->id() == id)
            return false;
    }
    if (qml_engine_)
    {
        ImageProvider *provider = new ImageProvider(name, uuid);
        qml_engine_->addImageProvider(provider->id(), provider);
        providers_.append(provider);
        emit providersChanged();
        emit sizeChanged();
    }
    return true;
}

ImageProvider *ImageProviderList::provider(const int index)
{
    if (index >= providers_.size())
        return nullptr;
    return providers_[index];
}

} // namespace quicktools::core
