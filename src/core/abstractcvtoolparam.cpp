#include "abstractcvtoolparam.h"

namespace quicktools::core {

bool AbstractCVInputParams::setInputImages(const QVariant &value)
{
    if (image_path_ == value)
    {
        return false;
    }
    QString path = value.toString();
    if (path.startsWith("file:///"))
    {
        image_path_ = path.right(path.size() - 8);
    }
    else
    {
        image_path_ = path;
    }
    emit inputImagesChanged();
    return true;
}

bool AbstractCVOutputParams::setOutputImages(const QVariant &value)
{
    if (output_data_ == value)
    {
        return false;
    }
    output_data_ = value;
    emit outputImagesChanged();
    return true;
}

} // namespace quicktools::core
