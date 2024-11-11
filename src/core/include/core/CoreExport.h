#pragma once

#ifdef _WIN32
#    define CORE_HIDDEN
#    define CORE_EXPORT __declspec(dllexport)
#    define CORE_IMPORT __declspec(dllimport)
#elif __GNUC__
#    define CORE_EXPORT __attribute__((__visibility__("default")))
#    define CORE_HIDDEN __attribute__((__visibility__("hidden")))
#    define CORE_IMPORT CORE_EXPORT
#else
#    define CORE_HIDDEN
#    define CORE_EXPORT
#    define CORE_IMPORT
#endif

#ifdef QUICKTOOLS_CORE_BUILD_SHARED_LIBS
#    define CORE_API CORE_EXPORT
#else
#    define CORE_API CORE_IMPORT
#endif
