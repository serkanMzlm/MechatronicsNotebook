# ==============================================================================
# CMakeLists.txt — C Learning Project
#
# Kapsanan komutlar:
#   cmake_minimum_required    → VERSION range
#   project                   → VERSION, DESCRIPTION, LANGUAGES, HOMEPAGE_URL
#   cmake_policy              → eski/yeni davranış kontrolü
#   option                    → kullanıcı tanımlı açma/kapama seçeneği
#   add_executable            → hedef tanımlama
#   target_include_directories → modern include yönetimi (PUBLIC/PRIVATE/INTERFACE)
#   target_link_libraries     → kütüphane bağlama
#   target_compile_options    → hedefe özel derleme bayrakları
#   target_compile_definitions → hedefe özel makro tanımları
#   set_target_properties     → C standardı, output adı, vs.
#   include                   → modül yükleme
# ==============================================================================

# ==============================================================================
# cmake_minimum_required
#
# VERSION range (3.12+): alt sınır zorunlu, üst sınır policy uyumluluğu için.
# Sadece alt sınır da verilebilir: VERSION 3.10
# Range syntax: VERSION 3.10...3.27
# ==============================================================================

cmake_minimum_required(VERSION 3.15...3.27)

# ==============================================================================
# project
#
# VERSION          → MAJOR.MINOR.PATCH.TWEAK formatında versiyon
#                    PROJECT_VERSION, PROJECT_VERSION_MAJOR vb. değişkenler otomatik set edilir
# DESCRIPTION      → PROJECT_DESCRIPTION değişkenine yazılır
# HOMEPAGE_URL     → PROJECT_HOMEPAGE_URL değişkenine yazılır
# LANGUAGES        → kullanılacak diller (C, CXX, ASM, NONE vb.)
#                    sadece C yazarak CXX derleyici kontrolü atlanır
# ==============================================================================

project(CLearningProject
    VERSION      1.0.0
    DESCRIPTION  "C Language Learning Project"
    LANGUAGES    C
)

message(STATUS "Project : ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Source  : ${CMAKE_SOURCE_DIR}")
message(STATUS "Build   : ${CMAKE_BINARY_DIR}")

# ==============================================================================
# cmake_policy
#
# CMake politikaları geriye dönük uyumluluğu yönetir.
# Her yeni CMake sürümü bazı davranışları değiştirir; policy ile kontrol edilir.
#
# CMP0074 → find_package() <PKG>_ROOT değişkenini arama yoluna ekler (3.12+)
# CMP0075 → CheckIncludeFile CMAKE_REQUIRED_LIBRARIES'i kullanır (3.12+)
# CMP0076 → target_sources() göreli yolları mutlak yola çevirir (3.13+)
#
# NEW → yeni davranışı aktif et
# OLD → eski davranışı koru (önerilmez, ileride kaldırılabilir)
# ==============================================================================

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0075 NEW)

# ==============================================================================
# option
#
# Kullanıcının cmake -DENABLE_X=ON gibi parametre geçebildiği bool seçeneği.
# cmake-gui ve ccmake'de de görünür.
#
# Kullanım: cmake -B build -DENABLE_ASAN=ON
# ==============================================================================

option(ENABLE_ASAN    "Enable AddressSanitizer"          OFF)
option(ENABLE_UBSAN   "Enable UndefinedBehaviorSanitizer" OFF)
option(BUILD_NETWORK  "Build network module"              ON)

# ==============================================================================
# CMAKE_MODULE_PATH — özel cmake modüllerinin aranacağı dizin
# ==============================================================================

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# ==============================================================================
# Modüller — her biri include edildiğinde otomatik çalışır
# ==============================================================================

include(Git)            # git_get_info() vb. çalışır
include(SystemCheck)    # run_system_checks() çalışır
include(DependencyCheck)# run_dependency_checks() çalışır
include(CustomTools)    # macro ve fonksiyonlar tanımlanır

# ==============================================================================
# C standardı — global ayar
#
# CMAKE_C_STANDARD: tüm hedefler için varsayılan C standardını belirler.
# set_target_properties ile hedef bazlı da ayarlanabilir (aşağıda gösterildi).
# ==============================================================================

set(CMAKE_C_STANDARD          11)
set(CMAKE_C_STANDARD_REQUIRED ON)   # standart desteklenmiyorsa hata ver
set(CMAKE_C_EXTENSIONS        OFF)  # GNU uzantılarını kapat (-std=c11, -std=gnu11 değil)

# ==============================================================================
# Global derleme bayrakları
#
# add_compile_options: tüm hedeflere uygulanır.
# Hedef bazlı kontrol için target_compile_options tercih edilir (aşağıda).
# ==============================================================================

add_compile_options(-Wall -Wextra -Wpedantic)

# ==============================================================================
# Hedef listesi — modül bazlı yönetim
#
# Her modül bağımsız bir executable olarak derlenir.
# Her birinin kendi main() fonksiyonu vardır.
# ==============================================================================

set(ALL_TARGETS "")

# ==============================================================================
# add_executable
#
# Derleme hedefi (executable) tanımlar.
# Her hedef bağımsızdır; kaynak dosyası, include ve flag'leri ayrı yönetilir.
# ==============================================================================

add_executable(basics_test         src/tutorial/basics.c)
add_executable(io_test             src/tutorial/io.c)
add_executable(pointers_test       src/tutorial/pointers.c)
add_executable(structs_test        src/tutorial/structs.c)
add_executable(functions_test      src/tutorial/functions.c)
add_executable(bitwise_test        src/tutorial/bitwise.c)
add_executable(strings_test        src/tutorial/strings.c)
add_executable(memory_test         src/tutorial/memory.c)
add_executable(preprocessor_test   src/tutorial/preprocessor.c)
add_executable(time_demo           src/tutorial/time_demo.c)
add_executable(algorithms          src/algorithms.c)

target_link_libraries(time_demo PRIVATE m)

list(APPEND ALL_TARGETS
    basics_test
    io_test
    pointers_test
    structs_test
    functions_test
    bitwise_test
    strings_test
    memory_test
    preprocessor_test
    time_demo
    algorithms
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

# ==============================================================================
# target_compile_definitions
#
# Hedefe özel makro tanımı ekler (-D bayrağı).
# add_compile_definitions'dan farkı: sadece belirtilen hedefe uygulanır.
# ==============================================================================

foreach(TARGET_NAME IN LISTS ALL_TARGETS)
    target_compile_definitions(${TARGET_NAME} PRIVATE
        PROJECT_NAME="${PROJECT_NAME}"
        PROJECT_VERSION="${PROJECT_VERSION}"
        $<$<CONFIG:Debug>:DEBUG_BUILD=1>      # generator expression: sadece Debug'da
    )
endforeach()

# ==============================================================================
# target_compile_options
#
# Hedefe özel derleme bayrakları ekler.
# add_compile_options'dan farkı: sadece belirtilen hedefe uygulanır.
# ==============================================================================

foreach(TARGET_NAME IN LISTS ALL_TARGETS)
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O2>
    )

    if(ENABLE_ASAN)
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=address)
        target_link_options(${TARGET_NAME}    PRIVATE -fsanitize=address)
    endif()

    if(ENABLE_UBSAN)
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=undefined)
        target_link_options(${TARGET_NAME}    PRIVATE -fsanitize=undefined)
    endif()
endforeach()

# ==============================================================================
# target_link_libraries
#
# Hedefe kütüphane bağlar.
# PUBLIC / PRIVATE / INTERFACE kapsamları burada da geçerlidir.
#
# CMake hedefleri (Threads::Threads) doğrudan kullanılabilir.
# ==============================================================================

foreach(TARGET_NAME IN LISTS ALL_TARGETS)
    target_link_libraries(${TARGET_NAME} PRIVATE
        Threads::Threads        # find_package(Threads) tarafından tanımlanır
        $<$<BOOL:${ZLIB_FOUND}>:ZLIB::ZLIB>  # ZLIB varsa bağla, yoksa atla
    )
endforeach()

# ==============================================================================
# set_target_properties
#
# Hedefin özelliklerini ayarlar.
#
# OUTPUT_NAME         → üretilen binary'nin adı (varsayılan: hedef adı)
# C_STANDARD          → bu hedef için C standardı (global ayarı ezer)
# C_STANDARD_REQUIRED → standart desteklenmiyorsa hata ver
# RUNTIME_OUTPUT_DIRECTORY → binary'nin yazılacağı dizin
# ==============================================================================

set_target_properties(basics_test PROPERTIES
    OUTPUT_NAME             "basics"
    C_STANDARD              11
    C_STANDARD_REQUIRED     ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

foreach(TARGET_NAME IN LISTS ALL_TARGETS)
    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()

# filter_existing_files: function versiyonu — PARENT_SCOPE ile değer döner
set(SOURCE_LIST
    "tutorial/basics.c"
    "algorithms.c"
)

filter_existing_files(SOURCE_LIST FILTERED_LIST)
message(STATUS "Filtered sources: ${FILTERED_LIST}")
message(STATUS "================================================")
message(STATUS "  ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  C Standard  : ${CMAKE_C_STANDARD}")
message(STATUS "  Build Type  : ${CMAKE_BUILD_TYPE}")
message(STATUS "  ASAN        : ${ENABLE_ASAN}")
message(STATUS "  UBSAN       : ${ENABLE_UBSAN}")
message(STATUS "  Targets     : ${ALL_TARGETS}")
message(STATUS "================================================")