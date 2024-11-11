#pragma once

#ifdef _WIN32
#    define DEEPLEARNING_HIDDEN
#    define DEEPLEARNING_EXPORT __declspec(dllexport)
#    define DEEPLEARNING_IMPORT __declspec(dllimport)
#elif __GNUC__
#    define DEEPLEARNING_EXPORT __attribute__((__visibility__("default")))
#    define DEEPLEARNING_HIDDEN __attribute__((__visibility__("hidden")))
#    define DEEPLEARNING_IMPORT DEEPLEARNING_EXPORT
#else
#    define DEEPLEARNING_HIDDEN
#    define DEEPLEARNING_EXPORT
#    define DEEPLEARNING_IMPORT
#endif

#ifdef QUICKTOOLS_DEEPLEARNING_BUILD_SHARED_LIBS
#    define DEEPLEARNING_API DEEPLEARNING_EXPORT
#else
#    define DEEPLEARNING_API DEEPLEARNING_IMPORT
#endif
