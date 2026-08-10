cmake_minimum_required(VERSION 3.8)
project(topic)

find_package(gz-cmake3 REQUIRED)

set(GZ_DISTRIBUTION "Garden")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

gz_find_package(gz-sim7 REQUIRED)
gz_find_package(gz-msgs9 REQUIRED)
gz_find_package(sdformat13 REQUIRED)
gz_find_package(gz-plugin2 REQUIRED COMPONENTS loader register)
gz_find_package(gz-transport12 VERSION 12.1 REQUIRED COMPONENTS log parameters)

set(GZ_PLUGIN_VER ${gz-plugin2_VERSION_MAJOR})
set(GZ_TRANSPORT_VER ${gz-transport12_VERSION_MAJOR})
set(GZ_SIM_VER ${gz-sim7_VERSION_MAJOR})
set(GZ_MSGS_VER ${gz-msgs9_VERSION_MAJOR})

add_executable(pub ${PROJECT_SOURCE_DIR}/publisher.cc)
add_executable(sub ${PROJECT_SOURCE_DIR}/subscriber.cc)

target_link_libraries(pub
  gz-plugin${GZ_PLUGIN_VER}::gz-plugin${GZ_PLUGIN_VER}
  gz-sim${GZ_SIM_VER}::gz-sim${GZ_SIM_VER}
  gz-transport${GZ_TRANSPORT_VER}::gz-transport${GZ_TRANSPORT_VER}
)
