cmake_minimum_required(VERSION 3.8)
project(cpp_service)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(client_node src/client.cpp)
add_executable(oop_client_node src/oop_client.cpp)
add_executable(server_node src/server.cpp)

ament_target_dependencies(client_node
  rclcpp
  std_msgs
  example_interfaces
)

ament_target_dependencies(server_node
  rclcpp
  std_msgs
  example_interfaces
)

ament_target_dependencies(oop_client_node
  rclcpp
  std_msgs
  example_interfaces
)

install(TARGETS
  client_node
  oop_client_node
  server_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
