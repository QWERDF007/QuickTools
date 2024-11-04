#pragma once

#include <QtCore/QtGlobal>

#if defined(QUICKTOOLS_SAMPLES_LIBRARY)
#    define QUICKTOOLS_SAMPLES_EXPORT Q_DECL_EXPORT
#else
#    define QUICKTOOLS_SAMPLES_EXPORT Q_DECL_IMPORT
#endif
