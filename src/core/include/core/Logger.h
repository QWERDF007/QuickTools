#pragma once

#include "core/CoreGlobal.h"

#include <spdlog/spdlog.h>

namespace quicktools::core {
QUICKTOOLS_CORE_EXPORT bool registerLogger(std::shared_ptr<spdlog::logger> logger);
} // namespace quicktools::core
