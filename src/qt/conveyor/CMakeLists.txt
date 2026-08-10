cmake_minimum_required(VERSION 3.16)

project(conveyor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Core SerialPort)

qt_standard_project_setup()

qt_add_executable(appconveyor
    main.cpp
    date_time.h
    date_time.cpp
    serial_comm.h
    serial_comm.cpp
    navigation_menu.h
    navigation_menu.cpp
)

qt_add_qml_module(appconveyor
    URI conveyor
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/NavigationMenu.qml
        qml/TopToolbar.qml
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

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appconveyor PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appconveyor
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appconveyor PRIVATE
    Qt6::Quick
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::SerialPort
)

install(TARGETS appconveyor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)































target_link_libraries(appconveyor
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appconveyor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
