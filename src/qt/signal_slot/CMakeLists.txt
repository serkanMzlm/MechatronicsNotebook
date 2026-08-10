cmake_minimum_required(VERSION 3.14)

project(signal_slot LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

include_directories(${PROJECT_SOURCE_DIR}/inc)

file(GLOB_RECURSE HEADER_FILE ${PROJECT_SOURCE_DIR}/inc/*.hpp)
file(GLOB_RECURSE SRC_FILE ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(signal_slot_lib ${SRC_FILE} ${HEADER_FILE})

add_executable(signal_slot
  main.cpp
)

target_link_libraries(signal_slot_lib PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(signal_slot signal_slot_lib)

include(GNUInstallDirs)
install(TARGETS signal_slot
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
