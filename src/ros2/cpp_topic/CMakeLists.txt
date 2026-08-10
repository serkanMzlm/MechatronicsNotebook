cmake_minimum_required(VERSION 3.8)
project(cpp_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(publisher_node src/publisher.cpp)
add_executable(subscriber_node src/subscriber.cpp)

ament_target_dependencies(publisher_node 
  rclcpp
  std_msgs
)

ament_target_dependencies(subscriber_node 
  rclcpp
  std_msgs
)

install(TARGETS
  publisher_node
  subscriber_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
