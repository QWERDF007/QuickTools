#include "deeplearning/detection/Yolov8.h"

#include "core/Error.h"

namespace quicktools::dl::detection {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

class Yolov8PythonInterface : public core::AbstractPythonInterface
{
public:
    Yolov8PythonInterface(QObject *parent = nullptr)
        : core::AbstractPythonInterface(parent)
    {
    }

protected:
    QString importModule() const override
    {
        return "";
    }
};

Yolov8Detection::Yolov8Detection(QObject *parent)
    : core::AbstractCVTool(parent)
{
}

std::tuple<int, QString> Yolov8Detection::doInProcess()
{
    auto algorithm_start_time = std::chrono::high_resolution_clock::now();

    int     ret{0};
    QString msg{"运行成功"};

    auto input_params  = getInputParams();
    auto output_params = getOutputParams();
    if (input_params == nullptr || output_params == nullptr)
        return {-1, tr("输入/输出参数为空指针")};

    auto algorithm_end_time = std::chrono::high_resolution_clock::now();
    auto algorithm_time = std::chrono::duration<double, std::milli>(algorithm_end_time - algorithm_start_time).count();
    addAlgorithmTime(algorithm_time);

    return {ret, msg};
}

QString Yolov8Detection::doc() const
{
    return "no doc";
}

int Yolov8Detection::initInputParams()
{
    if (input_params_)
    {
        input_params_->addParam("Image", tr("图像"), tr("输入图像的路径"), QuickToolParamType::ImageParamType,
                                QVariant(), QVariant(), true, true, true, true);
        input_params_->addParam("Image", tr("图像"), tr("输入图像的路径"), QuickToolParamType::ImageParamType,
                                QVariant(), QVariant(), true, true, true, true);
    }
    return Error::Success;
}

int Yolov8Detection::initOutputParams()
{
    if (output_params_)
    {
    }
    return Error::Success;
}

int Yolov8Detection::initSettings()
{
    if (settings_)
    {
    }
    return Error::Success;
}

} // namespace quicktools::dl::detection
