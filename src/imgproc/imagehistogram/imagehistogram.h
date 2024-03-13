#pragma once

#include "abstractcvtool.h"

#include <QObject>

namespace quicktools::imgproc {

class ImageHistogram : public core::AbstractCVTool
{
    Q_OBJECT
public:
    ImageHistogram(QObject *parent = nullptr);

    ~ImageHistogram() = default;

    int run() override;

    QString name() const override
    {
        return QString("图像直方图");
    }

private:
    int initInputParams() override;
    int initOutputParams() override;
};

REGISTER_CLASS(core::tooltypes::ImageHistogram, ImageHistogram)

} // namespace quicktools::imgproc
