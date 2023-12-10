
# 获取所有QML文件的相对路径
file(GLOB_RECURSE QML_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} qml/*.qml)
# 将 qml 文件组织到一起
source_group("QML Files" FILES ${QML_SOURCES} "qtquickcontrols2.conf")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

# 添加可执行文件
qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${QRC_SOURCES}
)

# 添加QML模块
qt_add_qml_module(${PROJECT_NAME}
    URI QuickTools
    VERSION 1.0
    QML_FILES ${QML_SOURCES}
)

# 添加资源文件
qt_add_resources(${PROJECT_NAME} "controls_conf"
    PREFIX "/"
    FILES "qtquickcontrols2.conf"
)


# 如果编译模式是Debug或RelWithDebInfo，则添加预处理定义QT_QML_DEBUG。用于开启Qt Quick调试功能
target_compile_definitions(${PROJECT_NAME}
PRIVATE 
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

# 链接库
target_link_libraries(${PROJECT_NAME}
PRIVATE 
    Qt6::Quick
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
