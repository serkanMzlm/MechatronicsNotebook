# ==============================================================================
# DependencyCheck.cmake — Bağımlılık Kontrolleri ve CMake Komut Örnekleri
#
# Kapsanan komutlar:
#   macro             → function farkı, ARGC, ARGN kullanımı
#   cmake_parse_arguments → isimli parametre desteği
#   find_package      → QUIET, REQUIRED, CONFIG modu
#   FetchContent      → FetchContent_Declare, FetchContent_MakeAvailable
#   math              → EXPR ile aritmetik
#   while             → koşullu döngü
#
# Kullanım (CMakeLists.txt içinde):
#   include(DependencyCheck)
# ==============================================================================

include(FetchContent)

# ==============================================================================
# log_dep — macro örneği
#
# macro ile function arasındaki temel fark:
#   - macro kendi kapsamı oluşturmaz, çağıranın kapsamında çalışır
#   - set() ile yazılan değişken doğrudan dışarıya geçer (PARENT_SCOPE gerekmez)
#   - return() kullanılamaz
#   - ARGC: toplam argüman sayısı
#   - ARGN: beklenen parametreler dışında gelen ek argümanlar
# ==============================================================================

macro(log_dep NAME)
    if(${NAME}_FOUND)
        message(STATUS "[Dep] ${NAME} — FOUND")
        # PARENT_SCOPE olmadan doğrudan dışarıya yazılır
        set(DEP_${NAME}_OK TRUE)
    else()
        message(WARNING "[Dep] ${NAME} — NOT FOUND (ARGC=${ARGC})")
        set(DEP_${NAME}_OK FALSE)
    endif()
endmacro()

# ==============================================================================
# check_dep — cmake_parse_arguments örneği (function)
#
# cmake_parse_arguments: fonksiyona isimli parametre desteği ekler.
#
# cmake_parse_arguments(<önek> <flag> <single> <multi> ${ARGN})
#   FLAG   → değer almaz        : REQUIRED
#   SINGLE → tek değer alır     : NAME
#   MULTI  → çok değer alır     : COMPONENTS
#
# Sonuçlar <önek>_<anahtar> değişkenine yazılır: ARG_NAME, ARG_REQUIRED ...
# ==============================================================================

function(check_dep)
    cmake_parse_arguments(ARG "REQUIRED" "NAME" "COMPONENTS" ${ARGN})

    if(NOT DEFINED ARG_NAME)
        message(FATAL_ERROR "[check_dep] NAME is required.")
    endif()

    find_package(${ARG_NAME} QUIET)
    log_dep(${ARG_NAME})

    if(NOT ${ARG_NAME}_FOUND AND ARG_REQUIRED)
        message(FATAL_ERROR "[Dep] Required package missing: ${ARG_NAME}")
    endif()
endfunction()

# ==============================================================================
# run_dependency_checks
# ==============================================================================

macro(run_dependency_checks)

    message(STATUS "--- Dependency Checks Starting ---")

    # find_package MODULE modu — REQUIRED: bulunamazsa derleme durur
    find_package(Threads REQUIRED)

    # find_package CONFIG modu — kütüphanenin kendi Config.cmake'ini arar
    find_package(GTest CONFIG QUIET)
    log_dep(GTest)

    # cmake_parse_arguments'lı fonksiyon kullanımı
    check_dep(NAME ZLIB    )
    check_dep(NAME Threads REQUIRED)

    # FetchContent: sistem kurulumu gerektirmeden dış bağımlılık indirir
    # find_package'dan farkı: kaynak olarak projeye dahil edilir
    FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG        v2.5.2
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(unity)

    # math EXPR: aritmetik işlem — sonuç her zaman integer'dır
    math(EXPR DEP_FLAGS "(1 << 0) | (1 << 1)")
    message(STATUS "[Dep] Flags: ${DEP_FLAGS}")

    # while: bağımlılık listesini kuyruk gibi işler
    set(DEP_QUEUE "ZLIB" "Threads" "GTest")
    list(LENGTH DEP_QUEUE QUEUE_LEN)

    while(QUEUE_LEN GREATER 0)
        list(GET       DEP_QUEUE 0 CURRENT)
        list(REMOVE_AT DEP_QUEUE 0)

        if(DEP_${CURRENT}_OK)
            message(STATUS "[Dep] ${CURRENT} — OK")
        else()
            message(STATUS "[Dep] ${CURRENT} — SKIPPED")
        endif()

        list(LENGTH DEP_QUEUE QUEUE_LEN)
    endwhile()

    message(STATUS "--- Dependency Checks Done ---")

endmacro()

run_dependency_checks()