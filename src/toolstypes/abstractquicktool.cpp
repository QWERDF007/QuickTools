#include "abstractquicktool.h"

namespace quicktools {



QVariant AbstractToolParams::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= params_.size())
        return QVariant();
    switch (role)
    {
    case ParamNameRole:

        break;
    default:
        break;
    }

    return QVariant();
}

bool AbstractToolParams::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}



bool AbstractQuickTool::setInputParams(AbstractToolInputParams *input_params)
{
    if (input_params_ != input_params)
    {
        input_params_ = input_params;
        emit inputParamsChanged();
        return true;
    }
    return false;
}



bool AbstractQuickTool::setOutputParams(AbstractToolOutputParams *output_params)
{
    if (output_params_ != output_params)
    {
        output_params_ = output_params;
        emit outputParamsChanged();
        return true;
    }
    return false;
}

} // namespace quicktools
