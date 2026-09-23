include("${CMAKE_CURRENT_LIST_DIR}/ConfigDependencyDefaults.cmake")

quicktools_dependency_variable_is_explicit(
    PYTHON_HOME QUICKTOOLS_DEPENDENCY_PYTHON_HOME _qt_python_home_explicit)
quicktools_dependency_variable_is_explicit(
    PYTHON_EXECUTABLE QUICKTOOLS_DEPENDENCY_PYTHON_EXECUTABLE _qt_python_exe_explicit)
quicktools_dependency_variable_is_explicit(
    Python_ROOT_DIR QUICKTOOLS_DEPENDENCY_PYTHON_ROOT _qt_python_root_explicit)

quicktools_dependency_default(python _qt_python_default_root)

set(_qt_python_root)
set(_qt_python_source)

if(_qt_python_home_explicit AND NOT PYTHON_HOME STREQUAL "")
    set(_qt_python_root "${PYTHON_HOME}")
    set(_qt_python_source "PYTHON_HOME")
elseif(_qt_python_exe_explicit AND NOT PYTHON_EXECUTABLE STREQUAL "")
    get_filename_component(_qt_python_root "${PYTHON_EXECUTABLE}" DIRECTORY)
    set(_qt_python_source "PYTHON_EXECUTABLE")
elseif(_qt_python_root_explicit AND NOT Python_ROOT_DIR STREQUAL "")
    set(_qt_python_root "${Python_ROOT_DIR}")
    set(_qt_python_source "Python_ROOT_DIR")
elseif(DEFINED ENV{PYTHON_HOME} AND NOT "$ENV{PYTHON_HOME}" STREQUAL "")
    set(_qt_python_root "$ENV{PYTHON_HOME}")
    set(_qt_python_source "environment")
elseif(_qt_python_default_root AND EXISTS "${_qt_python_default_root}")
    set(_qt_python_root "${_qt_python_default_root}")
    set(_qt_python_source "project-default")
elseif(DEFINED ENV{CONDA_PREFIX} AND NOT "$ENV{CONDA_PREFIX}" STREQUAL "")
    set(_qt_python_root "$ENV{CONDA_PREFIX}")
    set(_qt_python_source "environment")
endif()

if(_qt_python_root)
    quicktools_dependency_cache_set(
        PYTHON_HOME "${_qt_python_root}" PATH
        "Python installation directory"
        QUICKTOOLS_DEPENDENCY_PYTHON_HOME "${_qt_python_source}")

    if(WIN32)
        set(_qt_python_exe "${_qt_python_root}/python.exe")
    else()
        set(_qt_python_exe "${_qt_python_root}/bin/python")
    endif()

    set(PYTHON_EXECUTABLE "${_qt_python_exe}" CACHE FILEPATH "Python executable path" FORCE)
    set(Python_ROOT_DIR "${_qt_python_root}" CACHE PATH "Python root directory" FORCE)
    set(Python3_ROOT_DIR "${_qt_python_root}" CACHE PATH "Python root directory" FORCE)
    set(Python_EXECUTABLE "${_qt_python_exe}" CACHE FILEPATH "Python executable path" FORCE)
    set(Python3_EXECUTABLE "${_qt_python_exe}" CACHE FILEPATH "Python executable path" FORCE)
endif()

if(EXISTS "${PYTHON_EXECUTABLE}")
    message(STATUS "Found Python executable: ${PYTHON_EXECUTABLE}")
else()
    message(FATAL_ERROR "Python executable not found: ${PYTHON_EXECUTABLE}")
endif()