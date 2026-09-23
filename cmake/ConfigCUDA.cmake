include("${CMAKE_CURRENT_LIST_DIR}/ConfigDependencyDefaults.cmake")

quicktools_dependency_resolve_path(
    _qt_cuda_root _qt_cuda_origin cuda
    VARIABLES CUDAToolkit_ROOT CUDA_TOOLKIT_ROOT_DIR CUDA_PATH
    ENVIRONMENT_VARIABLES CUDA_PATH CUDA_TOOLKIT_ROOT_DIR CUDAToolkit_ROOT
)
if(_qt_cuda_root)
    if(NOT DEFINED CUDAToolkit_ROOT)
        quicktools_dependency_cache_set(
            CUDAToolkit_ROOT "${_qt_cuda_root}" PATH
            "CUDA Toolkit root directory"
            QUICKTOOLS_DEPENDENCY_CUDA_ROOT "${_qt_cuda_origin}")
    endif()
endif()

string(REPLACE "." ";" CUDA_VERSION_LIST ${CMAKE_CUDA_COMPILER_VERSION})
list(GET CUDA_VERSION_LIST 0 CUDA_VERSION_MAJOR)
list(GET CUDA_VERSION_LIST 1 CUDA_VERSION_MINOR)
list(GET CUDA_VERSION_LIST 2 CUDA_VERSION_PATCH)

find_package(CUDAToolkit ${CUDA_VERSION_MAJOR}.${CUDA_VERSION_MINOR} REQUIRED)
if(NOT DEFINED CUDAToolkit_BIN_DIR OR CUDAToolkit_BIN_DIR STREQUAL "")
    set(CUDAToolkit_BIN_DIR "${CUDAToolkit_TARGET_DIR}/bin" CACHE PATH "CUDA Toolkit runtime directory")
endif()

# CMake 3.20 的 FindCUDAToolkit 尚未提供 CUDA::nvml，在这里统一补齐该 target。
if(NOT TARGET CUDA::nvml)
    find_library(QUICKTOOLS_NVML_LIBRARY
        NAMES nvml nvidia-ml
        HINTS ${CUDAToolkit_LIBRARY_DIR}
    )
    if(QUICKTOOLS_NVML_LIBRARY)
        add_library(CUDA::nvml UNKNOWN IMPORTED)
        set_target_properties(CUDA::nvml PROPERTIES
            IMPORTED_LOCATION "${QUICKTOOLS_NVML_LIBRARY}"
        )
        target_include_directories(CUDA::nvml SYSTEM INTERFACE ${CUDAToolkit_INCLUDE_DIRS})
    endif()
endif()

if(CMAKE_CUDA_COMPILER_VERSION VERSION_LESS "11.8")
    message(FATAL_ERROR "Minimum CUDA version supported is 11.8")
endif()

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xfatbin=--compress-all")

if(NOT DEFINED ARCH_X86_64 AND NOT DEFINED ARCH_AARCH64)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
        set(ARCH_AARCH64 TRUE)
    else()
        set(ARCH_X86_64 TRUE)
    endif()
endif()

# see https://developer.nvidia.com/cuda-gpus
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES OR CMAKE_CUDA_ARCHITECTURES STREQUAL "")
    set(CMAKE_CUDA_ARCHITECTURES "$ENV{CUDAARCHS}")

    if(ARCH_X86_64)
        if(CMAKE_CUDA_COMPILER_VERSION VERSION_LESS "13.0")
            list(APPEND CMAKE_CUDA_ARCHITECTURES
                70-real # Volta  - gv100/Tesla
            )
        endif()
        if(CMAKE_CUDA_COMPILER_VERSION VERSION_GREATER_EQUAL "11.8")
            list(APPEND CMAKE_CUDA_ARCHITECTURES
                75-real # Turing - tu10x/GeForce
                80-real # Ampere - ga100/Tesla
                86-real # Ampere - ga10x/GeForce
                89-real # Ada    - ad102/GeForce
                90-real # Hopper - gh100/Tesla
            )
        endif()
    endif()

    if(CMAKE_CUDA_COMPILER_VERSION VERSION_LESS "13.0")
        list(APPEND CMAKE_CUDA_ARCHITECTURES 70-virtual)
    endif()

    set(CMAKE_CUDA_ARCHITECTURES "${CMAKE_CUDA_ARCHITECTURES}" CACHE STRING "CUDA architectures to build for")
endif()

find_library(CUDNN_LIB cudnn HINTS ${CUDAToolkit_LIBRARY_DIR})