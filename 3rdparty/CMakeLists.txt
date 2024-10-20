set(CMAKE_FOLDER 3rdparty)

# pybind11 -----------------------------
add_subdirectory(pybind11)

# sqlpp11 -----------------------------
set(BUILD_SQLITE3_CONNECTOR ON CACHE BOOL "Build sqlite3 connector")
add_subdirectory(sqlpp11)

# spdlog
add_subdirectory(spdlog)

# cpuinfo
set(CPUINFO_BUILD_BENCHMARKS OFF CACHE BOOL "Not Build cpufino benchmarks")
set(CPUINFO_BUILD_TOOLS OFF CACHE BOOL "Not Build cpufino commline tools")
set(CPUINFO_BUILD_UNIT_TESTS OFF CACHE BOOL "Not Build cpufino unit tests")
set(CPUINFO_BUILD_MOCK_TESTS OFF CACHE BOOL "Not Build cpufino mock tests")
add_subdirectory(cpuinfo)
