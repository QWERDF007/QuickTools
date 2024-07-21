#include "core/GlobalSettings.h"

#include "core/PythonManager.h"
#include "priv/Predefined.h"


namespace quicktools::core {

GlobalSettings *GlobalSettings::instance_ = nullptr;

GlobalSettings::GlobalSettings(QObject *parent)
    : AbstractQuickToolSettings(parent)
{
    addBasicSettings();
    connect(this, &AbstractQuickToolSettings::settingChanged, this, &GlobalSettings::onSettingChanged);
}

void GlobalSettings::addBasicSettings()
{
    addGroup(SettingsGroup::BasicGroup, tr("基础设置"));
    addToogleSwitchSetting(SettingsGroup::BasicGroup, Predefined::RUN_AFTER_CHANGED, tr("改变后运行"),
                           tr("输入参数改变后运行工具"), true);
    addGroup(SettingsGroup::PythonGroup, tr("Python 设置"));
    addFileFolderInputSetting(SettingsGroup::PythonGroup, Predefined::PYTHON_HOME, "PYTHON_HOME", "",
                              PythonManager::getInstance()->DefaultPythonHome(), false);
}

void GlobalSettings::onSettingChanged(const QString &key, const QVariant &value)
{
    if (key == Predefined::PYTHON_HOME)
    {
        Q_UNUSED(value)
        // TODO: 设置 PYTHON_HOME
    }
}

} // namespace quicktools::core
