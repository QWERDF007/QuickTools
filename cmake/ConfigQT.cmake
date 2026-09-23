include("${CMAKE_CURRENT_LIST_DIR}/ConfigDependencyDefaults.cmake")

quicktools_dependency_resolve_path(
    _qt_root _qt_origin qt
    VARIABLES Qt6_ROOT QT6_ROOT Qt_ROOT QT_ROOT QTDIR
    ENVIRONMENT_VARIABLES Qt6_ROOT QT6_ROOT Qt_ROOT QT_ROOT QTDIR
    PREFIX_PATHS ${CMAKE_PREFIX_PATH}
    REQUIRED_FILES lib/cmake/Qt6/Qt6Config.cmake
)
if(_qt_root)
    quicktools_dependency_cache_set(
        Qt6_ROOT "${_qt_root}" PATH
        "Qt6 installation root directory"
        QUICKTOOLS_DEPENDENCY_QT6_ROOT "${_qt_origin}")
endif()

if(DEFINED Qt6_ROOT AND NOT Qt6_ROOT STREQUAL "")
    set(Qt6_CMAKE_DIR "${Qt6_ROOT}/lib/cmake")
    set(Qt6_DIR "${Qt6_CMAKE_DIR}/Qt6")
    set(Qt6QmlTools_DIR "${Qt6_CMAKE_DIR}/Qt6QmlTools")
    set(QT_QML_IMPORT_DIR "${Qt6_ROOT}/qml")
    list(PREPEND CMAKE_PREFIX_PATH "${Qt6_ROOT}")
endif()

# 寻找Qt6的核心组件
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Widgets Charts)

# 如果开启测试，寻找Qt6测试组件
if(QUICKTOOLS_BUILD_TESTS)
    find_package(Qt6 REQUIRED COMPONENTS QuickTest)
endif()

if(TARGET Qt6::Core AND (NOT DEFINED QT_QML_IMPORT_DIR OR QT_QML_IMPORT_DIR STREQUAL ""))
    get_target_property(_qt6_core_location Qt6::Core LOCATION)
    if(_qt6_core_location)
        get_filename_component(_qt6_bin_dir "${_qt6_core_location}" DIRECTORY)
        if(IS_DIRECTORY "${_qt6_bin_dir}/../qml")
            get_filename_component(QT_QML_IMPORT_DIR "${_qt6_bin_dir}/../qml" ABSOLUTE)
        endif()
    endif()
endif()

# 设置Qt6项目
qt_standard_project_setup(REQUIRES 6)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 设置 QTP0001 策略为 NEW，即使使用新的资源前缀 :/qt/qml/ 作为默认值
qt_policy(SET QTP0001 NEW)
if(Qt6_VERSION VERSION_GREATER_EQUAL 6.8.0)
    qt_policy(SET QTP0004 NEW)
endif()

# 添加 qml 导入目录
# QML_IMPORT_PATH 用于支持语法高亮
list(APPEND QML_IMPORT_PATH ${CMAKE_BINARY_DIR})

if(DEFINED QT_QML_IMPORT_DIR AND NOT QT_QML_IMPORT_DIR STREQUAL "")
    list(APPEND QML_IMPORT_PATH ${QT_QML_IMPORT_DIR})
endif()

# 去除重复
list(REMOVE_DUPLICATES QML_IMPORT_PATH)

# 将导入目录添加到 cache, 强制更新
set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
    CACHE STRING "qml import paths"
    FORCE
)