#pragma once

#ifdef _WIN32
#    define IMGPROC_HIDDEN
#    define IMGPROC_EXPORT __declspec(dllexport)
#    define IMGPROC_IMPORT __declspec(dllimport)
#elif __GNUC__
#    define IMGPROC_EXPORT __attribute__((__visibility__("default")))
#    define IMGPROC_HIDDEN __attribute__((__visibility__("hidden")))
#    define IMGPROC_IMPORT IMGPROC_EXPORT
#else
#    define IMGPROC_HIDDEN
#    define IMGPROC_EXPORT
#    define IMGPROC_IMPORT
#endif

#ifdef QUICKTOOLS_IMGPROC_BUILD_SHARED_LIBS
#    define IMGPROC_API IMGPROC_EXPORT
#else
#    define IMGPROC_API IMGPROC_IMPORT
#endif
