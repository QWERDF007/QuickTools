
# 获取所有QML文件的相对路径
file(GLOB_RECURSE QML_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.qml)
# 将 qml 文件组织到一起
source_group("QML Files" FILES ${QML_SOURCES})

# 获取所有CPP文件的相对路径
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

# 获取所有头文件的相对路径
file(GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp) 

# 获取所有资源文件的相对路径
file(GLOB_RECURSE RES_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} */qmldir)
# 将 qmldir 组织到一起
source_group("Resource Files" FILES ${RES_FILES})

qt_add_big_resources(QRC_SOURCES ${PROJECT_SOURCE_DIR}/assets/assets.qrc)
# Have big_resource.qrc treated as a source file by Qt Creator
set_property(SOURCE ${PROJECT_SOURCE_DIR}/assets/assets.qrc PROPERTY SKIP_AUTORCC ON)


# 添加可执行文件
qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${QRC_SOURCES}
)

# 添加QML模块
qt_add_qml_module(${PROJECT_NAME}
    URI QuickTools
    VERSION 1.0
    QML_FILES ${QML_SOURCES}
    RESOURCES ${RES_FILES}
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
    Qt6::Gui
    QuickTools_types
    QuickTools_imgproc
    QuickTools_ui
)
