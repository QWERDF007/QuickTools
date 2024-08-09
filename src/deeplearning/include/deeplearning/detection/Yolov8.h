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
        return "yolov8";
    }

    QString doc() const override;

private:
    int initInputParams() override;
    int initOutputParams() override;
    int initSettings() override;

    static QString doc_;
};

REGISTER_QUICKTOOL(core::quicktooltype::Yolov8DetectionTool, Yolov8Detection)

} // namespace quicktools::dl::detection
