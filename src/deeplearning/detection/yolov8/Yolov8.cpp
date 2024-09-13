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
    setProgress(0.2);

    QList<QList<QVariant>>   cls;
    QList<QList<QVariant>>   conf;
    QList<core::CVToolShape> rects;
    // QList<QList<QVariant>> rects;

    pybind11::gil_scoped_acquire acquire;
    {
        // 初始化
        if (!python_interface_->obj)
        {
            python_interface_->obj = python_interface_->module.attr("Yolov8DetectionPredictor")(
                detection_params_.model_path.toUtf8().constData(), detection_params_.imgsz,
                detection_params_.device.toUtf8().constData());
            detection_params_.is_init = true;
        }
        // 重新初始化模型
        if (!detection_params_.is_init)
        {
            python_interface_->obj.attr("init_model")(detection_params_.model_path.toUtf8().constData(),
                                                      detection_params_.imgsz,
                                                      detection_params_.device.toUtf8().constData());
        }
        setProgress(0.5);
        //  检测
        pybind11::object res = python_interface_->obj.attr("predict")(PythonHelper::toNumpy<uint8_t>(image),
                                                                      detection_params_.conf, detection_params_.iou);
        QList<QVariant>  _cls;
        for (auto c : res["cls"])
        {
            _cls.append(c.cast<int>());
        }
        cls.append(_cls);
        QList<QVariant> _conf;
        for (auto v : res["conf"])
        {
            _conf.append(v.cast<double>());
        }
        conf.append(_conf);
        for (auto box : res["boxes"])
        {
            core::CVToolShape rect;
            QList<qreal>      data;
            // QList<QVariant> data;
            for (auto v : box)
            {
                data.append(v.cast<double>());
            }
            if (data.size() >= 4 && res["format"].cast<std::string>() == "xyxy")
            {
                data[2] = data[2] - data[0];
                data[3] = data[3] - data[1];
                rect.setShapeType(core::CVToolShape::Rectangle);
                rect.setData(data);
                // data[2] = data[2].toDouble() - data[0].toDouble();
                // data[3] = data[3].toDouble() - data[1].toDouble();
            }
            rects.append(rect);
        }
    }
    setProgress(0.9);

    auto algorithm_end_time = std::chrono::high_resolution_clock::now();
    auto algorithm_time = std::chrono::duration<double, std::milli>(algorithm_end_time - algorithm_start_time).count();
    addAlgorithmTime(algorithm_time);
    output_params->setData("Classes", QVariant::fromValue(cls));
    output_params->setData("Confidences", QVariant::fromValue(conf));
    output_params->shapesList()->setShapes(rects);
    output_params->setData("Rects", QVariant::fromValue(output_params->shapesList()));

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
                                "d:/models/yolov8/yolov8s.pt", QVariant(), true, true, true, true);
        input_params_->addIntSpinBox("Imgsz", tr("图像大小"), tr("模型的输入图像大小"), 640, 0, 10000, 1, true, true);
        // TODO: 获取推理设备列表
        input_params_->addComboBox("Device", tr("推理设备"), tr("模型的推理设备"), "cuda:0", QVariantList(), false,
                                   true);
        input_params_->addDoubleSpinBox("ConfidenceThreshold", tr("置信度阈值"), "", 0.25, 0, 1, 0.05, true, true);
        input_params_->addDoubleSpinBox("IouThreshold", tr("iou阈值"), "", 0.7, 0, 1, 0.05, 2, true, true);
    }
    return Error::Success;
}

int Yolov8Detection::initOutputParams()
{
    if (output_params_)
    {
        output_params_->addParam("Rects", tr("矩形框"), tr("模型预测的矩形框"), QuickToolParamType::ShapesListParamType,
                                 QVariant(), QVariant(), true, true);
        output_params_->addParam("Classes", tr("类别"), tr("模型预测的矩形框的类别"),
                                 QuickToolParamType::Int2DArrayParamType, QVariant(), QVariant(), false, true);
        output_params_->addParam("Confidences", tr("置信度"), tr("模型预测的矩形框的置信度"),
                                 QuickToolParamType::Double2DArrayParamType, QVariant(), QVariant(), false, true);
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
