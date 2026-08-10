cmake_minimum_required(VERSION 3.16)

project(command VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick REQUIRED SerialPort)

include_directories(${PROJECT_SOURCE_DIR}/inc)
file(GLOB_RECURSE SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE INC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appcommand
    main.cpp
    ${SRC_FILE}
    ${INC_FILE}
)

qt_add_qml_module(appcommand
    URI command
    VERSION 1.0
    QML_FILES
        recourse.qrc
        qml/ViewToolbar.qml
        qml/ViewWidgetLayer.qml
        qml/TestPage.qml
    RESOURCES
        recourse.qrc
)

set_target_properties(appcommand PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appcommand
    PRIVATE Qt6::Quick Qt6::SerialPort
)

include(GNUInstallDirs)
install(TARGETS appcommand
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
