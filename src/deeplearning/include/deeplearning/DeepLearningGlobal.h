#pragma once

#include <QtCore/QtGlobal>

// clang-format off

#if defined(QUICKTOOLS_DEEPLEARNING_LIBRARY)
#  define QUICKTOOLS_DEEPLEARNING_EXPORT Q_DECL_EXPORT
#else
#  define QUICKTOOLS_DEEPLEARNING_EXPORT Q_DECL_IMPORT
#endif

// clang-format on
