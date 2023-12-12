
# 获取所有QML文件的相对路径
file(GLOB_RECURSE QML_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.qml)
# 将 qml 文件组织到一起
source_group("QML Files" FILES ${QML_SOURCES} "qtquickcontrols2.conf")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

qt_add_big_resources(QRC_SOURCES ${PROJECT_SOURCE_DIR}/assets/assets.qrc)
# Have big_resource.qrc treated as a source file by Qt Creator
set_property(SOURCE ${PROJECT_SOURCE_DIR}/assets/assets.qrc PROPERTY SKIP_AUTORCC ON)


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
    QuickTools_types
    QuickTools_imgproc
    QuickTools_ui
)
