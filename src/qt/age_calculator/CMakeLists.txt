cmake_minimum_required(VERSION 3.14)

project(age_calculator LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(age_calculator
  main.cpp
  agecalc.cpp
  agecalc.h
)
target_link_libraries(age_calculator Qt${QT_VERSION_MAJOR}::Core)

install(TARGETS age_calculator
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
