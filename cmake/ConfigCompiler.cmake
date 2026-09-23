# 将C++标准设置为17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# 在RelWithDebInfo模式下给CXX编译器添加-O3和-ggdb参数
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O3 -ggdb")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -O3 -ggdb")

if(WARNINGS_AS_ERRORS)
    # 设置C语言警告为错误
    set(C_WARNING_ERROR_FLAG "-Werror")
    # 设置CUDA语言警告全部为错误
    set(CUDA_WARNING_ERROR_FLAG "-Werror all-warnings")
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8 /bigobj")
    set(C_WARNING_FLAGS "-W4")
else ()
    set(C_WARNING_FLAGS "-Wall -Wno-unknown-pragmas -Wpointer-arith -Wmissing-declarations -Wredundant-decls -Wmultichar -Wno-unused-local-typedefs -Wunused")
    set(CXX_WARNING_FLAGS "-Wsuggest-override")
    set(CUDA_WARNING_FLAGS "-Wno-tautological-compare")
endif ()

# 设置 C++ 和 C 编译标志
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_WARNING_ERROR_FLAG} ${C_WARNING_FLAGS} ${CXX_WARNING_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_WARNING_ERROR_FLAG} ${C_WARNING_FLAGS}")
# 设置 CUDA 编译标志
if (MSVC)
    add_definitions(-DNOMINMAX)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} ${CUDA_WARNING_ERROR_FLAG} ${CUDA_WARNING_FLAGS}")
else ()
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} ${CUDA_WARNING_ERROR_FLAG} ${C_WARNING_FLAGS} ${CXX_WARNING_FLAGS} ${CUDA_WARNING_FLAGS}")
endif ()

# 包含 CheckIPOSupported 模块
include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_SUPPORTED)
set(LTO_ENABLED ON)

# 开启 sanitizer 来检测代码问题 (支持 ENABLE_SANITIZER 与 QUICKTOOLS_ENABLE_SANITIZER)
if(ENABLE_SANITIZER OR QUICKTOOLS_ENABLE_SANITIZER)
    set(ENABLE_SANITIZER ON CACHE BOOL "Enabled sanitized build" FORCE)
    set(QUICKTOOLS_ENABLE_SANITIZER ON CACHE BOOL "Enabled sanitized build" FORCE)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(_sanitizer_compile_flags
            "-fsanitize=address -fsanitize-address-use-after-scope -fsanitize=leak -fsanitize=undefined -fno-sanitize-recover=all")
        set(_sanitizer_link_flags "-fsanitize=address -fsanitize=undefined")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_sanitizer_compile_flags}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_sanitizer_compile_flags}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${_sanitizer_link_flags}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${_sanitizer_link_flags}")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(_sanitizer_compile_flags
            "-fsanitize=address -fsanitize-address-use-after-scope -fsanitize=undefined -fno-sanitize-recover=all")
        set(_sanitizer_link_flags "-fsanitize=address -fsanitize=undefined")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_sanitizer_compile_flags}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_sanitizer_compile_flags}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${_sanitizer_link_flags}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${_sanitizer_link_flags}")
    elseif(MSVC)
        set(_sanitizer_compile_flags "/fsanitize=address")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_sanitizer_compile_flags}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_sanitizer_compile_flags}")
    endif()
endif()
