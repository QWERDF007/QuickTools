#pragma once

#include "CoreGlobal.h"

#include <opencv2/core.hpp>

#include <QQuickImageProvider>

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT ImageProvider : public QQuickImageProvider
{
    Q_OBJECT
    QML_ANONYMOUS
    Q_PROPERTY(QString url READ url CONSTANT FINAL)
public:
    ImageProvider(const QString &name, const QString &uuid);
    ~ImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    void setImage(const QString &path, const cv::Mat &image);

    cv::Mat image()
    {
        return image_;
    }

    QString id() const;

    QString url() const;

signals:
    void imageChanged();

private:
    QString name_;
    QString uuid_;
    QString path_;
    cv::Mat image_;
};

class QUICKTOOLS_CORE_EXPORT ImageProviderWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url CONSTANT FINAL)
public:
    ImageProviderWrapper(const QString &name, const QString &uuid, QObject *parent = nullptr,
                         QQmlEngine *qml_engine = nullptr, QJSEngine *js_engine = nullptr);
    ~ImageProviderWrapper();

    QString url() const
    {
        if (image_provider_)
            return image_provider_->url();
        return "";
    }

    void setPath(const QString &path)
    {
        path_ = path;
    }

    QString path() const
    {
        return path_;
    }

    void setImage(const QString &path, const cv::Mat &image)
    {
        if (image_provider_)
            image_provider_->setImage(path, image);
    }

    cv::Mat image()
    {
        if (image_provider_)
            return image_provider_->image();
        return cv::Mat();
    }

signals:
    void imageChanged();

private:
    QQmlEngine *qml_engine_{nullptr};
    QJSEngine  *js_engine_{nullptr};

    QString        path_;
    ImageProvider *image_provider_{nullptr};
};

class QUICKTOOLS_CORE_EXPORT ImageProviderList : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    Q_PROPERTY(QList<ImageProvider *> providers READ providers NOTIFY providersChanged FINAL)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged FINAL)
public:
    ImageProviderList(QObject *parent = nullptr, QQmlEngine *qml_engine = nullptr, QJSEngine *js_engine = nullptr);

    ~ImageProviderList();

    int size() const;

    QList<ImageProvider *> providers() const;
    bool                   addImageProvider(const QString &name, const QString &uuid);

    ImageProvider *provider(const int index);

    ImageProvider *at(const int index)
    {
        return provider(index);
    }

signals:
    void providersChanged();
    void sizeChanged();

private:
    QList<ImageProvider *> providers_;
    QQmlEngine            *qml_engine_{nullptr};
    QJSEngine             *js_engine_{nullptr};
};

} // namespace quicktools::core
