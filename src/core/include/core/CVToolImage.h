#pragma once

#include "CoreGlobal.h"

#include <opencv2/core.hpp>

#include <QQuickImageProvider>

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    ~ImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    void setImage(const cv::Mat &image);

private:
    cv::Mat image_;
};

class QUICKTOOLS_CORE_EXPORT CVToolImage : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    Q_PROPERTY(QString url READ getUrl NOTIFY urlChanged FINAL)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
public:
    CVToolImage(const QString &name, const QString& uuid, QObject *parent = nullptr, QQmlEngine * qml_engine = nullptr, QJSEngine* js_engine = nullptr);

    ~CVToolImage();

    void setImage(const QString& path, const cv::Mat & image);

    QString getUrl() const;

    QString path() const;
    bool setPath(const QString& path);

    ImageProvider* imageProvider()
    {
        return provider_;
    }

signals:
    void urlChanged();
    void pathChanged();
    void imageChanged();

private:
    QString name_;
    QString uuid_;
    QString path_;
    ImageProvider *provider_{nullptr};
    QQmlEngine *qml_engine_{nullptr};
    QJSEngine  *js_engine_{nullptr};
};

} // namespace quicktools::core
