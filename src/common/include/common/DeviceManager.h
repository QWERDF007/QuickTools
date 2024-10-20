#pragma once

#include "Singleton.h"

#include <QString>
#include <QStringList>

namespace quicktools::common {

class DeviceManager
{
    SINGLETON(DeviceManager)
public:
    int getCpusCount();
    int getGpusCount();

    QStringList getAllCpusNames();
    QStringList getAllGpusNames();

    QStringList getAllDevicesNames()
    {
        return getAllCpusNames() + getAllGpusNames();
    }

    static QString toYolo(const QString &device_name);

private:
    explicit DeviceManager();
    ~DeviceManager();

    bool cpuinfo_is_initialized_{false};
    bool nvml_is_initialized_{false};

    QStringList cpu_names_;
    QStringList gpu_names_;
};

} // namespace quicktools::common
