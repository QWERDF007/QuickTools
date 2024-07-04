set(CMAKE_FOLDER 3rdparty)

# pybind11 -----------------------------
add_subdirectory(pybind11)

# sqlpp11 -----------------------------
set(BUILD_SQLITE3_CONNECTOR ON CACHE BOOL "Build sqlite3 connector")
add_subdirectory(sqlpp11)