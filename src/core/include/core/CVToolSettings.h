#pragma once

#include "AbstractQuickToolSettings.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT CVToolSettings : public AbstractQuickToolSettings
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
