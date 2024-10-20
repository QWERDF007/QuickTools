#include "deeplearning/detection/Yolov8.h"

#include "common/DeviceManager.h"
#include "common/Utils.h"
#include "core/Error.h"
#include "core/PythonInterface.h"
#include "core/RuntimeParams.h"

#include <pybind11/pybind11.h>

#include <QFile>

namespace quicktools::dl::detection {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

using core::PythonHelper;

namespace {

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

class Yolov8DetectionRuntimeParams : public core::RuntimeParams
{
public:
    Yolov8DetectionRuntimeParams(QObject *parent = nullptr)
        : core::RuntimeParams(parent)
    {
        reader_.setNameFilters(common::FileReader::ImageFilters);
    }

    void reset() override;

    bool getInput(core::InputParams *input_params) override;
    bool genOutput(core::OutputParams *output_params) override;

    struct RuntimeInput
    {
        QString model_path;
        QString image_path;
        int     imgsz{640};
        QString device{"cuda:0"};
        double  conf_threshold{0.25};
        double  iou_threshold{0.7};
        bool    is_init{false};

        bool operator==(const RuntimeInput &other) const
        {
            return other.model_path == model_path && other.imgsz == imgsz && other.device == device;
        }

        bool operator!=(const RuntimeInput &other) const
        {
            return !operator==(other);
        }
    };

    struct RuntimeOutput
    {
        QList<QList<QVariant>>   cls;
        QList<QList<QVariant>>   conf;
        QList<core::CVToolShape> rects;
    };

    RuntimeInput  input;
    RuntimeOutput output;

private:
    common::FileReader reader_;

    friend class ::quicktools::dl::detection::Yolov8Detection;
};

bool Yolov8DetectionRuntimeParams::getInput(core::InputParams *input_params)
{
    if (input_params == nullptr)
    {
        setError(Error::InputParamsEmpty);
        return false;
    }
    RuntimeInput  new_input;
    const QString root   = input_params->data("Image", QuickToolParamRole::ParamValueRole).toString();
    new_input.image_path = reader_.read(root, true, true);
    if (new_input.image_path.isEmpty())
    {
        setError(Error::ImageFilePathEmpty);
        return false;
    }
    if (!QFile::exists(new_input.image_path))
    {
        setError(Error::FileNotFound, new_input.image_path);
        return false;
    }
    new_input.model_path = input_params->data("Model", QuickToolParamRole::ParamValueRole).toString();
    if (new_input.model_path.isEmpty())
    {
        setError(Error::ModelFilePathEmpty);
        return false;
    }
    if (!QFile::exists(new_input.model_path))
    {
        setError(Error::FileNotFound, new_input.model_path);
        return false;
    }
    new_input.imgsz = input_params->data("Imgsz", QuickToolParamRole::ParamValueRole).toInt();
    new_input.device
        = common::DeviceManager::toYolo(input_params->data("Device", QuickToolParamRole::ParamValueRole).toString());
    new_input.conf_threshold = input_params->data("ConfidenceThreshold", QuickToolParamRole::ParamValueRole).toDouble();
    new_input.iou_threshold  = input_params->data("IouThreshold", QuickToolParamRole::ParamValueRole).toDouble();

    // 相等只更新部分参数
    if (new_input == input)
    {
        input.image_path     = new_input.image_path;
        input.conf_threshold = new_input.conf_threshold;
        input.iou_threshold  = new_input.iou_threshold;
    }
    // 不相等则拷贝新参数, is_init = false
    else
    {
        input = new_input;
    }
    return true;
}

bool Yolov8DetectionRuntimeParams::genOutput(core::OutputParams *output_params)
{
    if (output_params == nullptr)
    {
        setError(Error::OutputParamsEmpty);
        return false;
    }
    output_params->setData("Classes", QVariant::fromValue(output.cls));
    output_params->setData("Confidences", QVariant::fromValue(output.conf));
    auto cv_output_params = dynamic_cast<core::CVOutputParams *>(output_params);
    if (cv_output_params)
    {
        cv_output_params->shapesListModel()->setShapes(output.rects); // 注意只能在主线程修改 model 的数据, 否则报错
        cv_output_params->setData("Rects", QVariant::fromValue(cv_output_params->shapesListModel()));
    }
    return true;
}

void Yolov8DetectionRuntimeParams::reset()
{
    output.cls.clear();
    output.conf.clear();
    output.rects.clear();
}

} // namespace

Yolov8Detection::Yolov8Detection(QObject *parent, QQmlEngine *qml_engine, QJSEngine *js_engine)
    : core::AbstractCVTool(parent, qml_engine, js_engine)
{
    python_interface_ = new Yolov8DetectionPythonInterface(this);
    runtime_params_   = new Yolov8DetectionRuntimeParams(this);
}

int Yolov8Detection::initInputParams()
{
    if (input_params_)
    {
        input_params_->addImage("Image", tr("图像"), tr("输入图像的路径"), QVariant(), true, true);
        input_params_->addParam("Model", tr("模型文件"), tr("模型文件的路径"), QuickToolParamType::InputFileParamType,
                                QVariant(), QVariant(), true, true, true, true);
        input_params_->addIntSpinBox("Imgsz", tr("图像大小"), tr("模型的输入图像大小"), 640, 0, 10000, true, true);
        QStringList all_devices    = common::DeviceManager::getInstance()->getAllDevicesNames();
        QString     default_device = all_devices.isEmpty() ? "" : all_devices.first();
        input_params_->addComboBox("Device", tr("推理设备"), tr("模型的推理设备"), default_device,
                                   QVariant::fromValue(all_devices), false, true);
        input_params_->addDoubleSpinBox("ConfidenceThreshold", tr("置信度阈值"), "", 0.25, 0, 1, 2, true, true);
        input_params_->addDoubleSpinBox("IouThreshold", tr("iou阈值"), "", 0.7, 0, 1, 2, true, true);
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

std::tuple<int, QString> Yolov8Detection::doInProcess()
{
    auto algorithm_start_time = std::chrono::high_resolution_clock::now();

    int     ret{0};
    QString msg{"运行成功"};

    Yolov8DetectionRuntimeParams *runtime = dynamic_cast<Yolov8DetectionRuntimeParams *>(runtime_params_);
    if (runtime == nullptr)
    {
        ret = Error::RuntimeParamsEmpty;
        msg = Error::ErrorGenerator::getErrorString(ret);
        return {ret, msg};
    }

    cv::Mat image = cv::imread(runtime->input.image_path.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);

    setProgress(0.2);

    pybind11::gil_scoped_acquire acquire;
    {
        // 初始化
        if (!python_interface_->obj)
        {
            python_interface_->obj = python_interface_->module.attr("Yolov8DetectionPredictor")(
                runtime->input.model_path.toUtf8().constData(), runtime->input.imgsz,
                runtime->input.device.toUtf8().constData());
            runtime->input.is_init = true;
        }
        // 重新初始化模型
        if (!runtime->input.is_init)
        {
            python_interface_->obj.attr("init_model")(runtime->input.model_path.toUtf8().constData(),
                                                      runtime->input.imgsz, runtime->input.device.toUtf8().constData());
        }
        setProgress(0.5);
        //  检测
        pybind11::object res = python_interface_->obj.attr("predict")(
            PythonHelper::toNumpy<uint8_t>(image), runtime->input.conf_threshold, runtime->input.iou_threshold);
        QList<QVariant> _cls;
        for (auto c : res["cls"])
        {
            _cls.append(c.cast<int>());
        }
        runtime->output.cls.append(_cls);

        QList<QVariant> _conf;
        for (auto v : res["conf"])
        {
            _conf.append(v.cast<double>());
        }
        runtime->output.conf.append(_conf);

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
            }
            runtime->output.rects.append(rect);
        }
    }

    setProgress(0.9);

    auto algorithm_end_time = std::chrono::high_resolution_clock::now();
    auto algorithm_time = std::chrono::duration<double, std::milli>(algorithm_end_time - algorithm_start_time).count();
    addAlgorithmTime(algorithm_time);

    return {ret, msg};
}

QString Yolov8Detection::doc() const
{
    return "no doc";
}

int Yolov8Detection::initSettings()
{
    if (settings_)
    {
    }
    return Error::Success;
}

} // namespace quicktools::dl::detection
