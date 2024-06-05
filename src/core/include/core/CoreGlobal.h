#pragma once

#include <QtCore/QtGlobal>

// clang-format off

#if defined(QUICKTOOLS_CORE_LIBRARY)
#  define QUICKTOOLS_CORE_EXPORT Q_DECL_EXPORT
#else
#  define QUICKTOOLS_CORE_EXPORT Q_DECL_IMPORT
#endif

// clang-format on
