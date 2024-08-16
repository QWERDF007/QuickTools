#include "deeplearning/detection/Yolov8.h"

#include "core/Error.h"

#include <pybind11/pybind11.h>

namespace quicktools::dl::detection {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

class Yolov8DetectionPythonInterface : public core::AbstractPythonInterface
{
public:
    Yolov8DetectionPythonInterface(QObject *parent = nullptr)
        : core::AbstractPythonInterface(parent)
    {
    }

protected:
    QString importModule() const override
    {
        return "yolov8_detection";
    }
};

Yolov8Detection::Yolov8Detection(QObject *parent)
    : core::AbstractCVTool(parent)
{
    python_interface_ = new Yolov8DetectionPythonInterface(this);
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
    pybind11::gil_scoped_acquire acquire;
    {
        if (!python_interface_->obj)
            python_interface_->obj = python_interface_->module.attr("Yolov8Detection")("", 640, "cuda:0");
        python_interface_->obj.attr("detect")("img");
    }
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
        input_params_->addParam("Image", tr("图像"), tr("输入图像的路径"), QuickToolParamType::InputImageParamType,
                                QVariant(), QVariant(), true, true, true, true);
        input_params_->addParam("Model", tr("模型文件"), tr("模型文件的路径"), QuickToolParamType::InputFileParamType,
                                QVariant(), QVariant(), true, true, true, true);
        input_params_->addParam("Imgsz", tr("图像大小"), tr("模型的输入图像大小"),
                                QuickToolParamType::IntSpinBoxParamType, QVariant(), QVariant(), true, true, true,
                                true);
        input_params_->addParam("ConfidenceThreshold", tr("置信度阈值"), "", QuickToolParamType::DoubleSpinBoxParamType,
                                QVariant(), QVariant(), true, true, true, true);
        input_params_->addParam("IouThreshold", tr("iou阈值"), "", QuickToolParamType::DoubleSpinBoxParamType,
                                QVariant(), QVariant(), true, true, true, true);
    }
    return Error::Success;
}

int Yolov8Detection::initOutputParams()
{
    if (output_params_)
    {
        output_params_->addParam("Rects", tr("矩形框"), tr("模型预测的矩形框"),
                                 QuickToolParamType::Double2DArrayParamType, QVariant(), QVariant(), false, true);
        output_params_->addParam("Classes", tr("类别"), tr("模型预测的矩形框的类别"),
                                 QuickToolParamType::Int1DArrayParamType, QVariant(), QVariant(), false, true);
        output_params_->addParam("Confidences", tr("置信度"), tr("模型预测的矩形框的置信度"),
                                 QuickToolParamType::Double1DArrayParamType, QVariant(), QVariant(), false, true);
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
