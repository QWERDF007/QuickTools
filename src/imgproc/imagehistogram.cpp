#include "imagehistogram.h"

#include <QtQml>


namespace quicktools {

namespace {

enum ParamRole
{
    NameRole = Qt::UserRole + 1,
};
} // namespace

ImageHistogram::ImageHistogram(QObject *parent)
    : AbstractQuickTool(parent)
{
}

int ImageHistogram::run()
{
    return 0;
}

} // namespace quicktools
