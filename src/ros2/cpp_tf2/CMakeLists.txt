cmake_minimum_required(VERSION 3.8)
project(cpp_tf2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(turtlesim REQUIRED)

add_executable(static_turtle_tf2_broadcaster src/static_turtle_tf2_broadcaster.cpp)

ament_target_dependencies(
   static_turtle_tf2_broadcaster
   geometry_msgs
   rclcpp
   tf2
   tf2_ros
)

install(TARGETS
   static_turtle_tf2_broadcaster
   DESTINATION lib/${PROJECT_NAME})

ament_package()
