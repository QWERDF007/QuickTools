#include "CVToolSettings.h"

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
    addGroup(SettingsGroup::DrawingGroup, "Drawing Settings");
    addColorDialogSetting(SettingsGroup::DrawingGroup, "ROIColor", tr("ROI Color"), "", QColor("red"));
    addColorDialogSetting(SettingsGroup::DrawingGroup, "ROIBorderColor", tr("ROI Border Color"), "", QColor("red"));
    addSliderSetting(SettingsGroup::DrawingGroup, "ROIColorAlpha", tr("ROI Color Alpha"), tr("alpha of the ROI color"),
                     0.5, 0.0, 1.0, 0.1);
}

} // namespace quicktools::core
