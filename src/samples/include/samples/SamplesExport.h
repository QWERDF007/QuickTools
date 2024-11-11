#pragma once

#ifdef _WIN32
#    define SAMPLES_HIDDEN
#    define SAMPLES_EXPORT __declspec(dllexport)
#    define SAMPLES_IMPORT __declspec(dllimport)
#elif __GNUC__
#    define SAMPLES_EXPORT __attribute__((__visibility__("default")))
#    define SAMPLES_HIDDEN __attribute__((__visibility__("hidden")))
#    define SAMPLES_IMPORT SAMPLES_EXPORT
#else
#    define SAMPLES_HIDDEN
#    define SAMPLES_EXPORT
#    define SAMPLES_IMPORT
#endif

#ifdef QUICKTOOLS_SAMPLES_BUILD_SHARED_LIBS
#    define SAMPLES_API SAMPLES_EXPORT
#else
#    define SAMPLES_API SAMPLES_IMPORT
#endif
