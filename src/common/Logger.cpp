#include "common/Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace quicktools::common {

std::shared_ptr<spdlog::logger> setupLogger(std::vector<spdlog::sink_ptr> sinks)
{
    static const std::string logger_name = "root";
    auto                     logger      = spdlog::get(logger_name);
    if (!logger)
    {
        if (sinks.size() > 0)
        {
            logger = std::make_shared<spdlog::logger>(logger_name, std::begin(sinks), std::end(sinks));
            spdlog::register_logger(logger);
            spdlog::set_default_logger(logger);
        }
        else
        {
            logger = spdlog::stdout_color_mt(logger_name);
        }
    }
    return logger;
}
} // namespace quicktools::common
