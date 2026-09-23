include("${CMAKE_CURRENT_LIST_DIR}/ConfigDependencyDefaults.cmake")

set(_qt_opencv_home_from_default OFF)
set(_qt_opencv_default_home)
set(_qt_opencv_search_dir)

if(DEFINED OpenCV_DIR AND "${OpenCV_DIR}" MATCHES "-NOTFOUND$")
    unset(OpenCV_DIR CACHE)
    unset(OpenCV_DIR)
endif()

quicktools_dependency_variable_is_explicit(
    OpenCV_DIR QUICKTOOLS_DEPENDENCY_OPENCV_DIR _qt_opencv_dir_explicit)
if(NOT _qt_opencv_dir_explicit)
    unset(OpenCV_DIR CACHE)
    unset(OpenCV_DIR)
endif()

if(_qt_opencv_dir_explicit)
    set(_qt_opencv_search_dir "${OpenCV_DIR}")
elseif(DEFINED ENV{OpenCV_DIR} AND NOT "$ENV{OpenCV_DIR}" STREQUAL "")
    set(_qt_opencv_search_dir "$ENV{OpenCV_DIR}")
    quicktools_dependency_cache_set(
        OpenCV_DIR "${_qt_opencv_search_dir}" PATH
        "OpenCV CMake package directory"
        QUICKTOOLS_DEPENDENCY_OPENCV_DIR environment)
else()
    quicktools_dependency_resolve_path(
        _qt_opencv_home _qt_opencv_home_origin opencv
        VARIABLES OpenCV_HOME OpenCV_ROOT
        ENVIRONMENT_VARIABLES OpenCV_HOME OpenCV_ROOT
    )
    if(_qt_opencv_home)
        quicktools_dependency_cache_set(
            OpenCV_HOME "${_qt_opencv_home}" PATH
            "OpenCV installation directory"
            QUICKTOOLS_DEPENDENCY_OPENCV_HOME "${_qt_opencv_home_origin}")
        set(_qt_opencv_search_dir "${OpenCV_HOME}/lib")
        if(EXISTS "${OpenCV_HOME}/lib/cmake/opencv4/OpenCVConfig.cmake")
            set(_qt_opencv_search_dir "${OpenCV_HOME}/lib/cmake/opencv4")
        endif()
        if(_qt_opencv_home_origin STREQUAL "project-default")
            set(_qt_opencv_home_from_default ON)
        endif()
    endif()
endif()

if(_qt_opencv_search_dir)
    if(_qt_opencv_dir_explicit)
        set(OpenCV_DIR "${_qt_opencv_search_dir}")
    elseif(NOT DEFINED OpenCV_DIR OR NOT OpenCV_DIR STREQUAL "${_qt_opencv_search_dir}")
        quicktools_dependency_cache_set(
            OpenCV_DIR "${_qt_opencv_search_dir}" PATH
            "OpenCV CMake package directory"
            QUICKTOOLS_DEPENDENCY_OPENCV_DIR
            "${_qt_opencv_home_origin}")
    endif()
endif()

if(DEFINED OpenCV_DIR AND NOT OpenCV_DIR STREQUAL "")
    set(OpenCV_LIBRARY_DIR "${OpenCV_DIR}")
endif()

find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
    if(_qt_opencv_home_from_default
       AND DEFINED OpenCV_LIB_PATH AND NOT OpenCV_LIB_PATH STREQUAL "")
        foreach(_qt_opencv_candidate IN ITEMS
                "${OpenCV_LIB_PATH}/.."
                "${OpenCV_LIB_PATH}/../.."
                "${OpenCV_LIB_PATH}/../../.."
                "${OpenCV_LIB_PATH}/../../../..")
            if(IS_DIRECTORY "${_qt_opencv_candidate}/include")
                get_filename_component(_qt_opencv_home "${_qt_opencv_candidate}" ABSOLUTE)
                unset(OpenCV_HOME CACHE)
                set(OpenCV_HOME "${_qt_opencv_home}" CACHE PATH
                    "OpenCV installation root")
                set(_qt_opencv_home_from_default OFF)
                break()
            endif()
        endforeach()
    endif()

    if(_qt_opencv_home_from_default AND OpenCV_DIR)
        foreach(_qt_opencv_candidate IN ITEMS
                "${OpenCV_DIR}"
                "${OpenCV_DIR}/.."
                "${OpenCV_DIR}/../.."
                "${OpenCV_DIR}/../../.."
                "${OpenCV_DIR}/../../../..")
            if(IS_DIRECTORY "${_qt_opencv_candidate}/bin")
                get_filename_component(_qt_opencv_home "${_qt_opencv_candidate}" ABSOLUTE)
                unset(OpenCV_HOME CACHE)
                set(OpenCV_HOME "${_qt_opencv_home}" CACHE PATH
                    "OpenCV installation root")
                break()
            endif()
        endforeach()
    endif()

    if((NOT DEFINED OpenCV_HOME OR OpenCV_HOME STREQUAL "") AND OpenCV_DIR)
        foreach(_qt_opencv_candidate IN ITEMS
                "${OpenCV_DIR}"
                "${OpenCV_DIR}/.."
                "${OpenCV_DIR}/../.."
                "${OpenCV_DIR}/../../.."
                "${OpenCV_DIR}/../../../..")
            if(IS_DIRECTORY "${_qt_opencv_candidate}/bin")
                get_filename_component(_qt_opencv_home "${_qt_opencv_candidate}" ABSOLUTE)
                set(OpenCV_HOME "${_qt_opencv_home}" CACHE PATH
                    "OpenCV installation root")
                break()
            endif()
        endforeach()
    endif()

    if((NOT DEFINED OpenCV_HOME OR OpenCV_HOME STREQUAL "")
       AND DEFINED OpenCV_LIB_PATH AND NOT OpenCV_LIB_PATH STREQUAL "")
        foreach(_qt_opencv_candidate IN ITEMS
                "${OpenCV_LIB_PATH}/.."
                "${OpenCV_LIB_PATH}/../.."
                "${OpenCV_LIB_PATH}/../../.."
                "${OpenCV_LIB_PATH}/../../../..")
            if(IS_DIRECTORY "${_qt_opencv_candidate}/include")
                get_filename_component(_qt_opencv_home "${_qt_opencv_candidate}" ABSOLUTE)
                set(OpenCV_HOME "${_qt_opencv_home}" CACHE PATH
                    "OpenCV installation root")
                break()
            endif()
        endforeach()
    endif()

    set(OpenCV_LIBRARY_DIR ${OpenCV_DIR})

    set(_qt_opencv_bin_dir)
    if(DEFINED OpenCV_LIB_PATH AND NOT OpenCV_LIB_PATH STREQUAL "")
        get_filename_component(_qt_opencv_bin_dir "${OpenCV_LIB_PATH}/../bin" ABSOLUTE)
        if(NOT IS_DIRECTORY "${_qt_opencv_bin_dir}")
            unset(_qt_opencv_bin_dir)
        endif()
    endif()
    if(NOT _qt_opencv_bin_dir AND DEFINED OpenCV_HOME
       AND IS_DIRECTORY "${OpenCV_HOME}/bin")
        get_filename_component(_qt_opencv_bin_dir "${OpenCV_HOME}/bin" ABSOLUTE)
    endif()
    if(NOT _qt_opencv_bin_dir AND OpenCV_DIR)
        foreach(_qt_opencv_candidate IN ITEMS
                "${OpenCV_DIR}/bin"
                "${OpenCV_DIR}/../bin"
                "${OpenCV_DIR}/../../bin"
                "${OpenCV_DIR}/../../../bin"
                "${OpenCV_DIR}/../../../../bin")
            if(IS_DIRECTORY "${_qt_opencv_candidate}")
                get_filename_component(_qt_opencv_bin_dir
                    "${_qt_opencv_candidate}" ABSOLUTE)
                break()
            endif()
        endforeach()
    endif()
    if(_qt_opencv_bin_dir
       AND (NOT DEFINED OpenCV_BIN_DIR OR OpenCV_BIN_DIR STREQUAL ""))
        set(OpenCV_BIN_DIR "${_qt_opencv_bin_dir}" CACHE PATH
            "OpenCV runtime binary directory")
    endif()

    unset(_qt_opencv_candidate)
    unset(_qt_opencv_home)
    unset(_qt_opencv_bin_dir)
endif()

unset(_qt_opencv_home_from_default)
unset(_qt_opencv_default_home)
unset(_qt_opencv_search_dir)
unset(_qt_opencv_home)
unset(_qt_opencv_home_origin)
unset(_qt_opencv_dir_explicit)
