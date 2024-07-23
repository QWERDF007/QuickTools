#include "core/GlobalSettings.h"

#include "core/PythonManager.h"
#include "core/QuickToolManager.h"
#include "priv/Predefined.h"

#include <spdlog/spdlog.h>

using namespace std::chrono_literals;

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
        QString python_home = value.toString();
        spdlog::warn("修改 PYTHON_HOME: {} -> {}", PythonManager::getInstance()->pythonHome().toUtf8().constData(),
                     python_home.toUtf8().constData());
        auto func = [python_home]()
        {
            std::vector<AbstractQuickTool *> python_tools;
            for (auto tool : QuickToolManager::getInstance()->activatedTools()->getActivatedTools())
            {
                if (!tool->hasPython())
                    continue;
                python_tools.push_back(tool);
                while (tool->isRunning()) std::this_thread::sleep_for(100ms);
            }
            qInfo() << __FUNCTION__ << __LINE__ << python_home;
            PythonManager::getInstance()->setPythonHome(python_home);
            for (auto tool : python_tools) tool->setIsInit(false);
        };
        QThreadPool::globalInstance()->start(func);
//        std::thread t(func);
//        t.detach();
    }
}

} // namespace quicktools::core
