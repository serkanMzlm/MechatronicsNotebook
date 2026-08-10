cmake_minimum_required(VERSION 3.8)
project(cpp_variable)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(variable REQUIRED)

add_executable(topic_node src/topic.cpp)
add_executable(client_node src/client.cpp)
add_executable(server_node src/server.cpp)

ament_target_dependencies(topic_node rclcpp variable)
ament_target_dependencies(client_node rclcpp variable)
ament_target_dependencies(server_node rclcpp variable)

install(TARGETS
  topic_node
  client_node
  server_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
