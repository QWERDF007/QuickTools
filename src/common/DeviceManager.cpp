#include "common/DeviceManager.h"

#include <cpuinfo.h>
#include <nvml.h>

#include <QRegularExpression>

namespace quicktools::common {

QStringList DeviceManager::getAllCpusNames()
{
    if (cpu_names_.empty() && cpuinfo_is_initialized_)
    {
        uint32_t device_count = cpuinfo_get_packages_count();
        for (uint32_t i = 0; i < device_count; ++i)
        {
            auto package = cpuinfo_get_package(i);
            if (package == nullptr)
                continue;
            QString device_name = QString("%1 (cpu:%2)").arg(package->name).arg(i);
            cpu_names_.append(device_name);
        }
    }
    return cpu_names_;
}

QStringList DeviceManager::getAllGpusNames()
{
    if (gpu_names_.empty() && nvml_is_initialized_)
    {
        uint32_t device_count{0};
        int      result = nvmlDeviceGetCount(&device_count);
        if (result != NVML_SUCCESS)
            return gpu_names_;
        for (uint32_t i = 0; i < device_count; ++i)
        {
            nvmlDevice_t device{nullptr};
            int          ret = nvmlDeviceGetHandleByIndex(i, &device);
            if (ret != NVML_SUCCESS)
                continue;
            char name[NVML_DEVICE_NAME_BUFFER_SIZE];
            ret = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);
            if (ret != NVML_SUCCESS)
                continue;
            QString device_name = QString("%1 (cuda:%2)").arg(name).arg(i);
            gpu_names_.append(device_name);
        }
    }
    return gpu_names_;
}

int DeviceManager::getCpusCount()
{
    if (cpuinfo_is_initialized_)
        return cpuinfo_get_packages_count();
    return 0;
}

QString DeviceManager::toYolo(const QString &device_name)
{
    // 正则表达式匹配 "(cpu:x)" 或 "(cuda:x)"
    static QRegularExpression device_expression{"\\((cpu|cuda):(\\d+)\\)"};
    QRegularExpressionMatch   match = device_expression.match(device_name);
    if (match.hasMatch())
    {
        QString device = match.captured(1);
        if (device == "cpu")
        {
            return device;
        }
        else if (device == "cuda")
        {
            int index = match.captured(2).toInt();
            return QString("cuda:%2").arg(index);
        }
    }
    return QString();
}

DeviceManager::DeviceManager()
{
    cpuinfo_is_initialized_ = cpuinfo_initialize();
    nvml_is_initialized_    = nvmlInit() == NVML_SUCCESS;
}

DeviceManager::~DeviceManager()
{
    if (cpuinfo_is_initialized_)
        cpuinfo_deinitialize();
}

} // namespace quicktools::common
