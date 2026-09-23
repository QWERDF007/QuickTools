include("${CMAKE_CURRENT_LIST_DIR}/ConfigDependencyDefaults.cmake")

quicktools_dependency_resolve_path(
    _qt_sqlite_root _qt_sqlite_origin sqlite
    VARIABLES SQLITE_ROOT SQLite3_ROOT SQLITE3_ROOT
    ENVIRONMENT_VARIABLES SQLITE_ROOT SQLite3_ROOT SQLITE3_ROOT
    PREFIX_PATHS ${CMAKE_PREFIX_PATH}
    REQUIRED_FILES include/sqlite3.h sqlite3.h
)
if(_qt_sqlite_root)
    quicktools_dependency_cache_set(
        SQLITE_ROOT "${_qt_sqlite_root}" PATH
        "SQLite installation root"
        QUICKTOOLS_DEPENDENCY_SQLITE_ROOT "${_qt_sqlite_origin}")
endif()

if(DEFINED SQLITE_ROOT AND NOT SQLITE_ROOT STREQUAL "")
    set(SQLite3_ROOT "${SQLITE_ROOT}" CACHE PATH "SQLite3 installation root")
    set(SQLITE3_ROOT "${SQLITE_ROOT}" CACHE PATH "SQLite3 installation root")
    list(PREPEND CMAKE_PREFIX_PATH "${SQLITE_ROOT}")

    foreach(_qt_sqlite_include_candidate IN ITEMS
            "${SQLITE_ROOT}/include"
            "${SQLITE_ROOT}")
        if(EXISTS "${_qt_sqlite_include_candidate}/sqlite3.h")
            set(SQLite3_INCLUDE_DIR "${_qt_sqlite_include_candidate}" CACHE PATH
                "SQLite3 include directory" FORCE)
            break()
        endif()
    endforeach()

    foreach(_qt_sqlite_library_candidate IN ITEMS
            "${SQLITE_ROOT}/lib/sqlite3.lib"
            "${SQLITE_ROOT}/sqlite3.lib"
            "${SQLITE_ROOT}/lib/libsqlite3.so"
            "${SQLITE_ROOT}/libsqlite3.so")
        if(EXISTS "${_qt_sqlite_library_candidate}")
            set(SQLite3_LIBRARY "${_qt_sqlite_library_candidate}" CACHE FILEPATH
                "SQLite3 library" FORCE)
            break()
        endif()
    endforeach()
endif()

find_package(SQLite3 QUIET)

if(NOT TARGET SQLite3::SQLite3)
    find_path(SQLite3_INCLUDE_DIR NAMES sqlite3.h
        HINTS ${SQLITE_ROOT}/include ${SQLITE_ROOT}
        PATH_SUFFIXES include
    )
    find_library(SQLite3_LIBRARY NAMES sqlite3 sqlite3_static
        HINTS ${SQLITE_ROOT}/lib ${SQLITE_ROOT}
        PATH_SUFFIXES lib lib64
    )
    if(SQLite3_INCLUDE_DIR AND SQLite3_LIBRARY)
        add_library(SQLite3::SQLite3 UNKNOWN IMPORTED GLOBAL)
        set_target_properties(SQLite3::SQLite3 PROPERTIES
            IMPORTED_LOCATION "${SQLite3_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${SQLite3_INCLUDE_DIR}"
        )
        set(SQLite3_FOUND TRUE)
    endif()
endif()

unset(_qt_sqlite_include_candidate)
unset(_qt_sqlite_library_candidate)