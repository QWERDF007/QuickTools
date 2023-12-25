#pragma once

#include "abstractquicktool.h"

#include <QObject>

namespace quicktools {

class ImageHistogram : public AbstractQuickTool
{
    Q_OBJECT
public:
    ImageHistogram(QObject *parent = nullptr);

    ~ImageHistogram() = default;


    int run() override;

private:

};

} // namespace quicktools
