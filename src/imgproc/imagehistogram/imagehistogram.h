#pragma once

#include "abstractquicktool.h"

#include <QObject>

namespace quicktools::imgproc {

class ImageHistogram : public AbstractQuickTool
{
    Q_OBJECT
public:
    ImageHistogram(QObject *parent = nullptr);

    ~ImageHistogram() = default;

    int run() override;

private:
};

REGISTER_CLASS(quicktooltypes::ImageHistogram, ImageHistogram)

} // namespace quicktools::imgproc
