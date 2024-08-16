#pragma once

#include <functional>

namespace quicktools::common {

class CrashHandler
{
public:
    explicit CrashHandler() = default;
    void setup(std::function<void()> crash_callback = nullptr);

private:
};

} // namespace quicktools::common
