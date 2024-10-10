#include "core/CVTool.h"
#include "core/QuickToolConfig.h"
#include "core/QuickToolManager.h"
#include "core/QuickToolType.h"

namespace quicktools::dl::detection {

class Yolov8Detection : public core::AbstractCVTool
{
public:
    Yolov8Detection(QObject *parent = nullptr, QQmlEngine * qml_engine = nullptr, QJSEngine* js_engine = nullptr);
    std::tuple<int, QString> doInProcess() override;

    QString name() const override
    {
        return "Yolov8Detection";
    }

    QString doc() const override;

private:
    int initInputParams() override;
    int initOutputParams() override;
    int initSettings() override;

    int checkInput();

    struct DetectionParams_t
    {
        QString model_path;
        QString image_path;
        int     imgsz{640};
        QString device{"cuda:0"};
        double  conf{0.25};
        double  iou{0.7};
        bool    is_init{false};

        bool operator==(const DetectionParams_t &other) const;
        bool operator!=(const DetectionParams_t &other) const;
    };

    DetectionParams_t detection_params_;

    static QString doc_;
};

class Yolov8DetectionConfig : public core::AbstractQuickToolConfig
{
public:
    QString title() const override
    {
        return "Yolov8 Detection";
    }

    QString url() const override
    {
        return "qrc:/qt/qml/QuickTools/deeplearning/detection/yolov8/Yolov8.qml";
    }

    QString desc() const override
    {
        return QString("Yolov8 目标检测");
    }

    QString icon() const override
    {
        return "/icons/detection";
    }

    QVariantMap extra() const override
    {
        return {
            {"recentlyUpdated", true},
            {  "recentlyAdded", true},
        };
    }

    int task() const override
    {
        return core::quicktooltasktype::QuickToolTaskType::DetectionTask;
    }

    int group() const override
    {
        return core::quicktoolgrouptype::QuickToolGroupType::DeepLearningGroup;
    }
};

REGISTER_QUICKTOOL(core::quicktooltype::Yolov8DetectionTool, Yolov8Detection, Yolov8DetectionConfig)

} // namespace quicktools::dl::detection
