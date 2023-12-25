#pragma once

#include <QtCore/QtGlobal>

// clang-format off

#if defined(QUICKTOOLS_TYPES_LIBRARY)
#  define QUICKTOOLS_TYPES_EXPORT Q_DECL_EXPORT
#else
#  define QUICKTOOLS_TYPES_EXPORT Q_DECL_IMPORT
#endif

// clang-format on
