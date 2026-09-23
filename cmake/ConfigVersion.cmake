# 必须在 include 时执行（而不是在函数调用时），以便获取本模块文件所在路径
get_filename_component(config_version_script_path ${CMAKE_CURRENT_LIST_FILE} PATH)

include(GetGitRevisionDescription)
# 获取当前仓库的 HEAD 引用（分支 refspec）以及提交哈希（commit）
get_git_head_revision(GIT_REFSPEC REPO_BRANCH REPO_COMMIT)

# 拼接工程版本号与可选后缀（例如 -rc1/-dev 等）
set(PROJECT_VERSION "${PROJECT_VERSION}${PROJECT_VERSION_SUFFIX}")

function(configure_version target LIBPREFIX incpath VERSION_FULL)
    # 为指定 target 配置版本信息，并生成/安装版本相关头文件。
    string(TOUPPER "${target}" TARGET)

    # 提取版本后缀（以 '-' 开头的部分，例如 1.2.3-rc1 -> rc1）
    string(REGEX MATCH "-(.*)$" version_suffix "${VERSION_FULL}")
    set(VERSION_SUFFIX ${CMAKE_MATCH_1})

    # 提取版本数字组件（major/minor/patch[/tweak]）
    string(REGEX MATCHALL "[0-9]+" version_list "${VERSION_FULL}")
    list(GET version_list 0 VERSION_MAJOR)
    list(GET version_list 1 VERSION_MINOR)
    list(GET version_list 2 VERSION_PATCH)

    list(LENGTH version_list num_version_components)

    # 允许 3 段或 4 段版本号
    if(num_version_components EQUAL 3)
        set(VERSION_TWEAK 0)
    elseif(num_version_components EQUAL 4)
        list(GET version_list 3 VERSION_TWEAK)
    else()
        message(FATAL_ERROR "Version must have either 3 or 4 components")
    endif()

    # API 版本编码：major*100 + minor
    math(EXPR VERSION_API_CODE "${VERSION_MAJOR}*100 + ${VERSION_MINOR}")

    string(REPLACE "-" "_" tmp ${VERSION_FULL})
    # 构建版本字符串：<版本号>-<构建后缀>
    set(VERSION_BUILD "${tmp}-${QUICKTOOLS_BUILD_SUFFIX}")
    # cmake 构建时间
    string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S %z")

    # 由模板生成版本头文件（写入 build/include 目录），供编译与安装使用
    configure_file(${config_version_script_path}/VersionDef.h.in include/${incpath}/VersionDef.h @ONLY ESCAPE_QUOTES)
    configure_file(${config_version_script_path}/VersionUtils.h.in include/${incpath}/detail/VersionUtils.h @ONLY ESCAPE_QUOTES)

    # 将版本信息缓存为 INTERNAL
    set(${LIBPREFIX}_VERSION_FULL ${VERSION_FULL} CACHE INTERNAL "${TARGET} full version")
    set(${LIBPREFIX}_VERSION_MAJOR ${VERSION_MAJOR} CACHE INTERNAL "${TARGET} major version")
    set(${LIBPREFIX}_VERSION_MINOR ${VERSION_MINOR} CACHE INTERNAL "${TARGET} minor version")
    set(${LIBPREFIX}_VERSION_PATCH ${VERSION_PATCH} CACHE INTERNAL "${TARGET} patch version")
    set(${LIBPREFIX}_VERSION_TWEAK ${VERSION_TWEAK} CACHE INTERNAL "${TARGET} tweak version")
    set(${LIBPREFIX}_VERSION_SUFFIX ${VERSION_SUFFIX} CACHE INTERNAL "${TARGET} version suffix")
    set(${LIBPREFIX}_VERSION_API ${VERSION_MAJOR}.${VERSION_MINOR} CACHE INTERNAL "${TARGET} API version")
    set(${LIBPREFIX}_VERSION_API_CODE ${VERSION_API_CODE} CACHE INTERNAL "${TARGET} API code")
    set(${LIBPREFIX}_VERSION_BUILD ${VERSION_BUILD} CACHE INTERNAL "${TARGET} build version")
    set(${LIBPREFIX}_BUILD_TIME ${BUILD_TIME} CACHE INTERNAL "${TARGET} build time")

    # 让 target 在构建时能找到生成的头文件（build/include）
    target_include_directories(${target}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/${incpath}/VersionDef.h
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${incpath}
            COMPONENT dev)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/${incpath}/detail/VersionUtils.h
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${incpath}/detail
            COMPONENT dev)
endfunction()
