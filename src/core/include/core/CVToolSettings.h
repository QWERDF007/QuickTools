#pragma once

#include "QuickToolSettings.h"

namespace quicktools::core {

class CORE_API CVToolSettings : public AbstractQuickToolSettings
{
public:
    CVToolSettings(QObject *parent = nullptr);
    ~CVToolSettings();

    QString name() const override
    {
        return "CVToolSettings";
    }

private:
    void addCVBasicSettings();
};

} // namespace quicktools::core
