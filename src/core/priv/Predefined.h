#pragma once
#include <QString>

#define DEBUG_MSG

#ifdef DEBUG_MSG
#    define Critical(MSG) qCritical() << __FUNCTION__ << __LINE__ << "msg:" << MSG
#else
#    define Critical(MSG)
#endif

namespace quicktools::core {
const QString RUN_AFTER_CHANGED = "RunAfterChanged";
} // namespace quicktools::core
