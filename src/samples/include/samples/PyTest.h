#pragma once
#include "core/QuickTool.h"
#include "core/QuickToolConfig.h"
#include "core/QuickToolManager.h"
#include "core/QuickToolType.h"


namespace quicktools::samples {

class PyTest : public core::AbstractTool<core::InputParams, core::OutputParams, core::QuickToolSettings>
{
public:
    PyTest(QObject *parent = nullptr, QQmlEngine * qml_engine = nullptr, QJSEngine* js_engine = nullptr);

    QString name() const override
    {
        return "PyTest";
    }

    std::tuple<int, QString> doInProcess() override;

    QString doc() const override
    {
        return "No Doc";
    }

protected:
    int initInputParams() override;
    int initOutputParams() override;
    int initSettings() override;

private:
};

class PyTestConfig : public core::AbstractQuickToolConfig
{
public:
    QString title() const override
    {
        return "PyTest";
    }

    QString url() const override
    {
        return "qrc:/qt/qml/QuickTools/samples/PyTest/PyTest.qml";
    }

    QString desc() const override
    {
        return QString("Python 测试工具");
    }

    QString icon() const override
    {
        return "/icons/testing";
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
        return core::quicktooltasktype::QuickToolTaskType::EmptyTask;
    }

    int group() const override
    {
        return core::quicktoolgrouptype::QuickToolGroupType::SamplesGroup;
    }
};

REGISTER_QUICKTOOL(core::quicktooltype::PyTestTool, PyTest, PyTestConfig)
} // namespace quicktools::samples
