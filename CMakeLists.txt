cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "QuickTools")
project(${PROJECT_NAME}
        LANGUAGES C CXX
        VERSION 0.0.1
        DESCRIPTION "一些工具集合"
)

# MSVC 下所有目标统一使用动态运行库
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# 确保 cuda 主机编译器与我们使用的一致
if(NOT CMAKE_CUDA_HOST_COMPILER)
    set(CMAKE_CUDA_HOST_COMPILER "${CMAKE_CXX_COMPILER}")
endif()

enable_language(CUDA)

# Used when creating special builds
set(PROJECT_VERSION_SUFFIX "-beta")

# 如果用户没指定安装前缀
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" CACHE PATH "where ${PROJECT_NAME} will be installed" FORCE)
endif()

# 配置构建树的选项
option(QUICKTOOLS_BUILD_TESTS "Enable testsuite" OFF)
option(QUICKTOOLS_BUILD_DOCS "Build Docs" OFF)
option(QUICKTOOLS_ENABLE_SANITIZER "Enabled sanitized build" OFF)
option(ENABLE_SANITIZER "Enabled sanitized build" OFF)
if(ENABLE_SANITIZER)
    set(QUICKTOOLS_ENABLE_SANITIZER ON CACHE BOOL "Enabled sanitized build" FORCE)
elseif(QUICKTOOLS_ENABLE_SANITIZER)
    set(ENABLE_SANITIZER ON CACHE BOOL "Enabled sanitized build" FORCE)
endif()

# 配置构建树
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ConfigVersion)
include(ConfigBuildTree)
include(ConfigCompiler)
include(ConfigCUDA)
include(ConfigQT)
include(ConfigOpenCV)
include(ConfigSQLite)
include(ConfigPython)

# 第三方库
add_subdirectory(3rdparty EXCLUDE_FROM_ALL)

add_subdirectory(src)

# 构建测试用例
if(QUICKTOOLS_BUILD_TESTS)
    enable_testing()
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        add_subdirectory(tests)
    endif()
endif()

message(STATUS "")
include(PrintConfig)
