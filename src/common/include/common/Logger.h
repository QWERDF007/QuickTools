#pragma once

#include <spdlog/spdlog.h>

namespace quicktools::common {
std::shared_ptr<spdlog::logger> setupLogger(std::vector<spdlog::sink_ptr> sinks);
} // namespace quicktools::common
