#include "CVToolSettings.h"
#include <QColor>

namespace quicktools::core {

CVToolSettings::CVToolSettings(QObject *parent) : AbstractQuickToolSettings(parent)
{
    copyFrom(GlobalSettings::getInstance(), SettingsGroup::BasicGroup);
    addCVBasicSettings();
}

CVToolSettings::~CVToolSettings()
{

}

void CVToolSettings::addCVBasicSettings()
{
    addGroup(SettingsGroup::DrawingGroup, "Drawing Settings");
    addSetting(SettingsGroup::DrawingGroup, "ROI color", tr("ROI color"), SettingsType::ColorDialogType, QColor("red"));
}

} // namespace quicktools::core
