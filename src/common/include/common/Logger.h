#pragma once

#include <spdlog/spdlog.h>

namespace quicktools::common {
std::vector<spdlog::sink_ptr> defaultSinks();
std::shared_ptr<spdlog::logger> setupLogger(const std::vector<spdlog::sink_ptr>& sinks);
} // namespace quicktools::common
