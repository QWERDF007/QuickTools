#include "common/CrashHandler.h"

#if defined(_WIN32)
#    include "common/WindowsCCrashHandler.h"
#else
#    include "common/LinuxCCrashHandler.h"
#endif

namespace quicktools::common {

void CrashHandler::setup()
{
#if defined(_WIN32)
    WindowsCCrashHandler ccrash_handler;
    ccrash_handler.setup();
#else
    LinuxCCrashHandler ccrash_handler;
    ccrash_handler.setup();
#endif
}

} // namespace quicktools::common
