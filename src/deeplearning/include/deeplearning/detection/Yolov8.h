#include "core/CVTool.h"
#include "core/QuickToolManager.h"
#include "core/QuickToolType.h"

namespace quicktools::dl::detection {

class Yolov8Detection : public core::AbstractCVTool
{
public:
    Yolov8Detection(QObject* parent=nullptr);
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

    struct DetectionParams_t{
        QString model_path;
        QString image_path;
        int imgsz{640};
        QString device{"cuda:0"};
        double conf{0.25};
        double iou{0.7};
        bool is_init{false};
        bool operator==(const DetectionParams_t& other) const;
        bool operator!=(const DetectionParams_t& other) const;
    };

    DetectionParams_t detection_params_;

    static QString doc_;
};

REGISTER_QUICKTOOL(core::quicktooltype::Yolov8DetectionTool, Yolov8Detection)

} // namespace quicktools::dl::detection
