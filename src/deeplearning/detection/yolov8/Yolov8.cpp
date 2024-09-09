#include "deeplearning/detection/Yolov8.h"

#include "core/Error.h"

#include <pybind11/pybind11.h>

namespace quicktools::dl::detection {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

using core::PythonHelper;

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

    ret = checkInput();
    if (ret != Error::Success)
    {
        msg = Error::ErrorGenerator::getErrorString(ret);
        return {ret, msg};
    }
    auto output_params = getOutputParams();
    if (output_params == nullptr)
        return {-1, tr("输出参数为空指针")};
    cv::Mat image = cv::imread(detection_params_.image_path.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);
    pybind11::gil_scoped_acquire acquire;
    {
        // 初始化
        if (!python_interface_->obj)
        {
            python_interface_->obj = python_interface_->module.attr("Yolov8Detection")(
                detection_params_.model_path.toUtf8().constData(), detection_params_.imgsz,
                detection_params_.device.toUtf8().constData());
            detection_params_.is_init = true;
        }
        // 重新初始化模型
        if (!detection_params_.is_init)
        {
            python_interface_->obj.attr("init_model")(detection_params_.model_path, detection_params_.imgsz,
                                                      detection_params_.device);
        }
        //  检测
        pybind11::object res = python_interface_->obj.attr("detect")(PythonHelper::toNumpy<uint8_t>(image),
                                                                     detection_params_.conf, detection_params_.iou);
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
                                QuickToolParamType::IntSpinBoxParamType, 640, QVariant(), true, true, true, true);
        // TODO: 获取推理设备列表
        input_params_->addComboBox("Device", tr("推理设备"), tr("模型的推理设备"), "cpu", QVariantList(), false, true);
        input_params_->addParam("ConfidenceThreshold", tr("置信度阈值"), "", QuickToolParamType::DoubleSpinBoxParamType,
                                0.25, QVariant(), true, true, true, true);
        input_params_->addParam("IouThreshold", tr("iou阈值"), "", QuickToolParamType::DoubleSpinBoxParamType, 0.7,
                                QVariant(), true, true, true, true);
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

int Yolov8Detection::checkInput()
{
    auto input_params = getInputParams();
    if (input_params == nullptr)
        return Error::InputParamsEmpty;
    DetectionParams_t new_params;
    new_params.image_path = input_params->data("Image", QuickToolParamRole::ParamValueRole).toString();
    if (new_params.image_path.isEmpty())
        return Error::InputImageEmpty;
    new_params.model_path = input_params->data("Model", QuickToolParamRole::ParamValueRole).toString();
    if (new_params.model_path.isEmpty())
        return Error::ModelFileEmpty;
    new_params.imgsz  = input_params->data("Imgsz", QuickToolParamRole::ParamValueRole).toInt();
    new_params.device = input_params->data("Device", QuickToolParamRole::ParamValueRole).toString();
    // 不相等则拷贝新参数, is_init = false
    if (new_params == detection_params_)
        detection_params_.image_path = new_params.image_path;
    else
        detection_params_ = new_params;
    return Error::Success;
}

bool Yolov8Detection::DetectionParams_t::operator==(const DetectionParams_t &other) const
{
    return other.model_path == model_path && other.imgsz == imgsz && other.device == device;
}

bool Yolov8Detection::DetectionParams_t::operator!=(const DetectionParams_t &other) const
{
    return !operator==(other);
}

} // namespace quicktools::dl::detection
