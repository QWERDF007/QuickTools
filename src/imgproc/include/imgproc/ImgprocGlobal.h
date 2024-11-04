#pragma once

#include <QtCore/QtGlobal>

#if defined(QUICKTOOLS_IMGPROC_LIBRARY)
#    define QUICKTOOLS_IMGPROC_EXPORT Q_DECL_EXPORT
#else
#    define QUICKTOOLS_IMGPROC_EXPORT Q_DECL_IMPORT
#endif
