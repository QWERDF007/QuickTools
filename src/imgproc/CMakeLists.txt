set(PLUGIN_NAME "imgproc")
set(TARGET_NAME "${PROJECT_NAME}_${PLUGIN_NAME}")

# 将名称转换为大写
string(TOUPPER ${TARGET_NAME} TARGET_NAME_UPPER)
# 设置额外编译定义
set(TARGET_EXPORTS "${TARGET_NAME_UPPER}_LIBRARY")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 获取所有源文件的相对路径
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp) 

# 获取所有头文件的相对路径
file(GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp) 

# 获取所有QML文件的相对路径
file(GLOB_RECURSE QML_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.qml)
# 将 qml 文件组织到一起
source_group("QML Files" FILES ${QML_SOURCES})

# 设置插件输出目录
set(QML_PLUGIN_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${PLUGIN_NAME}")

# 添加 qml 导入目录
# QML_IMPORT_PATH 用于支持语法高亮
list(APPEND QML_IMPORT_PATH ${CMAKE_BINARY_DIR})

# 去除重复
list(REMOVE_DUPLICATES QML_IMPORT_PATH)

# 将导入目录添加到 cache, 强制更新
set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
    CACHE STRING "qml import paths"
    FORCE
)

# 添加qml模块
qt_add_library(${TARGET_NAME} SHARED
    ${SOURCES}
    ${HEADERS}
    ${QML_SOURCES}
)
qt_add_qml_module(${TARGET_NAME}
    # 没有下面这行代码就会生成 xxxplugin.dll 与 xxxpluginplugin.dll 两个动态库
    PLUGIN_TARGET ${TARGET_NAME}
    OUTPUT_DIRECTORY ${QML_PLUGIN_DIRECTORY}
    VERSION 1.0
    URI "${PROJECT_NAME}.${PLUGIN_NAME}"
    SOURCES ${SOURCES}
    QML_FILES ${QML_SOURCES}
    # RESOURCES ${resource_files}
)

# 如果编译模式是Debug或RelWithDebInfo，则添加预处理定义QT_QML_DEBUG。用于开启Qt Quick调试功能
target_compile_definitions(${TARGET_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        Qt6::Core Qt6::Gui Qt6::Quick
        QuickTools_core QuickTools_core_headers QuickTools_ui
)

# 添加一个宏定义, 配合动态库导出函数、类
# https://stackoverflow.com/a/67923443
add_compile_definitions(${TARGET_NAME} PRIVATE ${TARGET_EXPORTS})

# 添加接口头文件，链接目标后可以include, 无需另外包含头文件目录
set(PLUGIN_HEADER "${PROJECT_NAME}_${PLUGIN_NAME}_headers")
add_library(${PROJECT_NAME}_${PLUGIN_NAME}_headers INTERFACE)
target_include_directories(${PLUGIN_HEADER}
    INTERFACE .
)



# 下面这部分用于复制 dll 和 qmldir, 需要将 dll 和 qmldir 添加到 QQmlEngine::importPathList 中
# 否则无法使用插件, 默认会将可执行文件所在目录添加到列表中, 所以将插件拷贝到该目录下
# 注意 win 是在使用 vscode 进行 cmake 后构建, 使用 qtcreator 可能生成目录不同, 需进行修改

# vscode
# 设置将要复制的目标目录, 稍后创建该目录。如果是windows则根据配置在目录添加Debug/Release前缀
set(DLL_COPY_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<BOOL:${WIN32}>:$<CONFIG>>/${PROJECT_NAME}/${PLUGIN_NAME}")
# 设置目标 dll 文件路径。如果是windows则根据配置在目录添加Debug/Release前缀，且如果是Debug则添加后缀
set(DLL_FILE "${QML_PLUGIN_DIRECTORY}/$<$<BOOL:${WIN32}>:$<CONFIG>>/${TARGET_NAME}$<$<CONFIG:DEBUG>:${CMAKE_DEBUG_POSTFIX}>.dll")

# qtcreator
#set(DLL_COPY_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<$<BOOL:${WIN32}>:$<CONFIG>>/${PROJECT_NAME}/${PLUGIN_NAME}")
#set(DLL_FILE "${QML_PLUGIN_DIRECTORY}/$<$<BOOL:${WIN32}>:$<CONFIG>>/${TARGET_NAME}$<$<CONFIG:DEBUG>:${CMAKE_DEBUG_POSTFIX}>.dll")

# 设置 qmldir 文件路径
set(QMLDIR_FILE ${QML_PLUGIN_DIRECTORY}/qmldir)

# 添加自定义命令, POST_BUILD: 指定目标其他规则执行完后执行, copy: 复制, copy_if_different: 输入有变则复制
add_custom_command(TARGET ${TARGET_NAME}
POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Creating directory '${DLL_COPY_DIR}'"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DLL_COPY_DIR}
    COMMAND ${CMAKE_COMMAND} -E echo "Copying '${DLL_FILE}' and '${QMLDIR_FILE}' to '${DLL_COPY_DIR}'"
    COMMAND ${CMAKE_COMMAND} -E copy ${DLL_FILE} ${QMLDIR_FILE} ${DLL_COPY_DIR}
)
