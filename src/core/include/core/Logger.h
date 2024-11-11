#pragma once

#include "core/CoreExport.h"

#include <spdlog/spdlog.h>

namespace quicktools::core {
CORE_API bool registerLogger(std::shared_ptr<spdlog::logger> logger);
} // namespace quicktools::core
