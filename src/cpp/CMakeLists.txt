cmake_minimum_required(VERSION 3.15...3.27)

project(CLearningProject
    VERSION      1.0.0
    DESCRIPTION  "Cpp Language Learning Project"
    LANGUAGES    CXX C
)

message(STATUS "Project : ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Source  : ${CMAKE_SOURCE_DIR}")
message(STATUS "Build   : ${CMAKE_BINARY_DIR}")

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0075 NEW)

# CMakeLists.txt içinde şunu bul:
set(CMAKE_CXX_STANDARD 17)

# → Değiştir:
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

set(ALL_TARGETS "")

add_executable(basics_test            src/tutorial/basics.cpp)
add_executable(smart_pointers_test    src/tutorial/smart_pointers.cpp)
add_executable(type_casting_test      src/tutorial/type_casting.cpp)
add_executable(loading_sim            src/loading_sim.cpp)

list(APPEND ALL_TARGETS
    basics_test
    smart_pointers_test
    type_casting_test
    loading_sim
)


# ==============================================================================
# target_include_directories
#
# include_directories'den farkı: sadece belirtilen hedefe uygulanır.
#
# PUBLIC     → bu hedef ve bu hedefe bağlı hedefler kullanır
# PRIVATE    → sadece bu hedef kullanır (bağımlı hedeflere geçmez)
# INTERFACE  → bu hedef kullanmaz, sadece bağımlı hedefler kullanır
# ==============================================================================

foreach(TARGET_NAME IN LISTS ALL_TARGETS)
    target_include_directories(${TARGET_NAME}
        PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_BINARY_DIR}/include   # generate_version_header() çıktısı
    )
endforeach()


foreach(TARGET_NAME IN LISTS ALL_TARGETS)
    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()