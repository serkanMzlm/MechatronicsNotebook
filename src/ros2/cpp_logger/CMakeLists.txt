cmake_minimum_required(VERSION 3.8)
project(cpp_logger)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(logger_node src/logger_node.cpp)

ament_target_dependencies(
  logger_node
  rclcpp
)

install(
  TARGETS
  logger_node
  DESTINATION
  lib/${PROJECT_NAME}
)

ament_package()
