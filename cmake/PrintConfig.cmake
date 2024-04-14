message(STATUS "General configuration for ${PROJECT_NAME}")
message(STATUS "version: ${PROJECT_VERSION}")
message(STATUS "Build options")

if(WARNINGS_AS_ERRORS)
    message(STATUS "    WARNINGS_AS_ERRORS       : ON")
else()
    message(STATUS "    WARNINGS_AS_ERRORS       : OFF")
endif()


if(QUICKTOOLS_BUILD_TESTS)
    message(STATUS "    QUICKTOOLS_BUILD_TESTS   : ON")
else()
    message(STATUS "    QUICKTOOLS_BUILD_TESTS   : OFF")
endif()

if(QUICKTOOLS_BUILD_DOCS)
    message(STATUS "    QUICKTOOLS_BUILD_DOCS    : ON")
else()
    message(STATUS "    QUICKTOOLS_BUILD_DOCS    : OFF")
endif()


# Compilation

message(STATUS "")
message(STATUS "Platform")
message(STATUS "    Host             : ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION} ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "    Target           : ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "    CMake            : ${CMAKE_VERSION}")
message(STATUS "    CMake generator  : ${CMAKE_GENERATOR}")
message(STATUS "    CMake build tool : ${CMAKE_BUILD_TOOL}")
message(STATUS "    Configuration    : ${CMAKE_BUILD_TYPE}")
message(STATUS "    install          : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "    ccache           : ${CCACHE_EXEC}")
message(STATUS "    ccache stats log : ${CCACHE_STATSLOG}")

message(STATUS "")

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
get_directory_property(dir_defs_ COMPILE_DEFINITIONS)
set(dir_defs "")
foreach(def ${dir_defs_})
    set(dir_defs "${dir_defs} -D${def}")
endforeach()
get_directory_property(dir_opt COMPILE_OPTIONS)

message(STATUS "Compiler/linker config")
message(STATUS "    C++ Compiler : ${CMAKE_CXX_COMPILER} (${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS "    C++ Standard : ${CMAKE_CXX_STANDARD}")
message(STATUS "    C++ Flags    : ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${BUILD_TYPE}}")
message(STATUS "    CMAKE_POSITION_INDEPENDENT_CODE    : ${CMAKE_POSITION_INDEPENDENT_CODE}")
message(STATUS "")
message(STATUS "    C Compiler   : ${CMAKE_C_COMPILER} (${CMAKE_C_COMPILER_VERSION})")
message(STATUS "    C Flags      : ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${BUILD_TYPE}}")
message(STATUS "")
message(STATUS "    CUDA Compiler           : ${CMAKE_CUDA_COMPILER} (${CMAKE_CUDA_COMPILER_VERSION})")
message(STATUS "    CUDA Arch               : ${CMAKE_CUDA_ARCHITECTURES}")
message(STATUS "    CUDA flags              : ${CMAKE_CUDA_FLAGS} ${CMAKE_CUDA_FLAGS_${BUILD_TYPE}}")
message(STATUS "    CUDA toolkit target dir : ${CUDAToolkit_TARGET_DIR}")
message(STATUS "")
message(STATUS "    Compiler Options    : ${dir_opt}")
message(STATUS "    Definitions         : ${dir_defs}")
message(STATUS "")
message(STATUS "    Linker flags (exec) : ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_${BUILD_TYPE}}")
message(STATUS "    Linker flags (lib)  : ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_${BUILD_TYPE}}")
message(STATUS "")
message(STATUS "    Link-time optim.    : supported ${LTO_SUPPORTED}, enabled ${LTO_ENABLED}")
message(STATUS "")


message(STATUS "3rdparty")
message(STATUS "OpenCV")
message(STATUS "    OpenCV version     : ${OpenCV_VERSION}")
message(STATUS "    OpenCV bin dir     : ${OpenCV_BIN_DIR}")
message(STATUS "    OpenCV include dir : ${OpenCV_INCLUDE_DIRS}")
message(STATUS "    OpenCV lib dir     : ${OpenCV_LIBRARY_DIR}")
message(STATUS "    OpenCV libs        : ${OpenCV_LIBS}")
message(STATUS "")

message(STATUS "CUDA")
message(STATUS "    CUDAToolkit version     : ${CUDAToolkit_VERSION}")
message(STATUS "    CUDAToolkit root        : ${CUDAToolkit_TARGET_DIR}")
message(STATUS "    CUDAToolkit bin dir     : ${CUDAToolkit_BIN_DIR}")
message(STATUS "    CUDAToolkit include dir : ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")
message(STATUS "    CUDAToolkit include dir : ${CUDAToolkit_INCLUDE_DIRS}")
message(STATUS "    CUDAToolkit lib root    : ${CUDAToolkit_LIBRARY_ROOT}")
message(STATUS "    CUDAToolkit lib dir     : ${CUDAToolkit_LIBRARY_DIR}")
message(STATUS "    cudnn lib               : ${CUDNN_LIB}")
message(STATUS "")

message(STATUS "Qt")
message(STATUS "    Qt version          : ${Qt6Core_VERSION}")
message(STATUS "    Qt core lib         : ${Qt6Core_LIBRARIES}")
message(STATUS "    Qt core include dir : ${Qt6Core_INCLUDE_DIRS}")
message(STATUS "    Qt libs             : ${Qt6Core_LIBRARIES} ${Qt6Gui_LIBRARIES} ${Qt6Quick_LIBRARIES} ${Qt6QuickTest_LIBRARIES}")
message(STATUS "")
