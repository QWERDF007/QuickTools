#pragma once

#include <QtCore/QtGlobal>

// clang-format off

#if defined(QUICKTOOLS_UI_LIBRARY)
#  define QUICKTOOLS_UI_EXPORT Q_DECL_EXPORT
#else
#  define QUICKTOOLS_UI_EXPORT Q_DECL_IMPORT
#endif

// clang-format on
