cmake_minimum_required(VERSION 3.5)

project(ros2_topic LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(ament_cmake REQUIRED)

if(ANDROID)
  add_library(ros2_topic SHARED
    main.cpp
    window.cpp
    window.h
    window.ui
  )
else()
  add_executable(ros2_topic
    main.cpp
    window.cpp
    rosnode.cpp
    rosnode.h
    window.h
    window.ui
  )
endif()

target_link_libraries(ros2_topic  Qt5::Widgets)
ament_target_dependencies(ros2_topic rclcpp example_interfaces)
