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

} // namespace quicktools::core
