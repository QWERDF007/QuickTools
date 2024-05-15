#include "CrashHandler.h"

#if defined(_WIN32)
#    include "WindowsCCrashHandler.h"
#else
#    include "LinuxCCrashHandler.h"
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