#pragma once

#include "AbstractCVTool.h"

#include <opencv2/core.hpp>

#include <QObject>

namespace quicktools::imgproc {

class ImageHistogram : public core::AbstractCVTool
{
    Q_OBJECT
public:
    ImageHistogram(QObject *parent = nullptr);

    ~ImageHistogram() = default;

    std::tuple<int, QString> exec() override;

    QString name() const override
    {
        return QString("图像直方图");
    }

    QString doc() const override;

private:
    int initInputParams() override;
    int initOutputParams() override;

    int cvtColor(const cv::Mat &src, cv::Mat &dst, const QString &color_space);

    static QString doc_;
};

REGISTER_CLASS(core::tooltypes::ImageHistogram, ImageHistogram)

} // namespace quicktools::imgproc
