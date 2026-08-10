cmake_minimum_required(VERSION 3.16)

project(conveyor_2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Core SerialPort)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appconveyor_2
    main.cpp
    date_time.h date_time.cpp
    section_navigation_menu.h section_navigation_menu.cpp
    serial_comm.h serial_comm.cpp

)

qt_add_qml_module(appconveyor_2
    URI conveyor_2
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/NavigationMenu.qml
        qml/TopToolbar.qml
        qml/TestModeMenu.qml
    RESOURCES
        assets/icons/home.png
        assets/icons/service.png
        assets/icons/settings.png
        assets/icons/warning.png
        assets/icons/play.png
        assets/icons/reset.png
        assets/icons/pause.png
        assets/icons/test.png
        assets/icons/conveyor.ico
)

set_target_properties(appconveyor_2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appconveyor_2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appconveyor_2
    PRIVATE Qt6::Quick
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::SerialPort
)

include(GNUInstallDirs)
install(TARGETS appconveyor_2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
