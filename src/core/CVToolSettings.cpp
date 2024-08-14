#include "core/CVToolSettings.h"

#include "core/GlobalSettings.h"

#include <QColor>

namespace quicktools::core {

CVToolSettings::CVToolSettings(QObject *parent)
    : AbstractQuickToolSettings(parent)
{
    copyFrom(GlobalSettings::getInstance(), SettingsGroup::BasicGroup);
    addCVBasicSettings();
}

CVToolSettings::~CVToolSettings() {}

void CVToolSettings::addCVBasicSettings()
{
    addGroup(SettingsGroup::DrawingGroup, tr("绘制设置"));
    addColorDialogSetting(SettingsGroup::DrawingGroup, "ROIColor", tr("ROI 颜色"), "", QColor("#ff08ff"));
    addColorDialogSetting(SettingsGroup::DrawingGroup, "ROIBorderColor", tr("ROI 边框颜色"), "", QColor("#ff08ff"));
    addSliderSetting(SettingsGroup::DrawingGroup, "ROIColorAlpha", tr("ROI 颜色 Alpha"),
                     tr("ROI 颜色的 Alpha 数值, 修改 ROI 的不透明度"), 0.1, 0.0, 1.0, 0.05);
}

} // namespace quicktools::core
