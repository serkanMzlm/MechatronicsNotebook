# ==============================================================================
# SystemCheck.cmake — Sistem Ortamı Kontrolleri ve CMake Komut Örnekleri
#
# Kapsanan komutlar:
#   include(Check*)               → CheckIncludeFile, CheckFunctionExists,
#                                   CheckSymbolExists, CheckTypeSize,
#                                   CheckCSourceCompiles, CheckCSourceRuns
#   cmake_host_system_information → CPU, RAM, OS, platform bilgisi
#   add_compile_definitions       → global derleme tanımları
#   set                           → temel, CACHE, ENV, unset
#   list                          → APPEND, LENGTH, GET, FIND,
#                                   REMOVE_DUPLICATES, SORT, JOIN, FILTER, TRANSFORM
#   foreach                       → IN LISTS, RANGE, ZIP_LISTS
#
# Kullanım (CMakeLists.txt içinde):
#   include(SystemCheck)          → run_system_checks() otomatik çalışır
# ==============================================================================

include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckCSourceCompiles)
include(CheckCSourceRuns)

function(run_system_checks)

    # ==========================================================================
    # set / unset
    #
    # CACHE BOOL   → cmake-gui / ccmake'de kullanıcı tarafından değiştirilebilir
    # ENV          → $ENV{} ile ortam değişkeni okunur
    # unset        → değişkeni tanımsız hale getirir
    # ==========================================================================

    set(CHECK_RESULTS "")

    # CACHE: yapılandırma arayüzünde görünür, bir kez yazılır sonra korunur
    set(SYSTEM_CHECK_STRICT OFF CACHE BOOL "Treat check failures as fatal errors")

    # ENV: ortam değişkeni okuma — yoksa boş string döner
    set(HOST_CC_ENV "$ENV{CC}")

    # unset: geçici değişkeni siler
    set(TEMP "geçici")
    unset(TEMP)

    # ==========================================================================
    # cmake_host_system_information
    #
    # Derleme yapılan makinenin donanım ve OS bilgilerini sorgular.
    # Cross-compile senaryolarında HOST ile TARGET birbirinden farklıdır.
    # ==========================================================================

    cmake_host_system_information(RESULT HOST_OS       QUERY OS_NAME)
    cmake_host_system_information(RESULT HOST_PLATFORM QUERY OS_PLATFORM)
    cmake_host_system_information(RESULT HOST_CORES    QUERY NUMBER_OF_LOGICAL_CORES)
    cmake_host_system_information(RESULT HOST_RAM      QUERY TOTAL_PHYSICAL_MEMORY)
    cmake_host_system_information(RESULT HOST_NAME     QUERY HOSTNAME)

    message(STATUS "  Host : ${HOST_NAME} | ${HOST_OS} | ${HOST_PLATFORM}")
    message(STATUS "  HW   : ${HOST_CORES} cores | ${HOST_RAM} MiB RAM")

    # add_compile_definitions: tüm hedeflere global olarak tanım ekler
    # target_compile_definitions'dan farkı: belirli bir hedefe değil, herkese uygulanır
    add_compile_definitions(HOST_OS="${HOST_OS}" HOST_PLATFORM="${HOST_PLATFORM}")

    # ==========================================================================
    # CheckIncludeFile
    # Header dosyasının derleyici tarafından bulunup bulunamadığını test eder.
    # Sonuç cache'e yazılır → cmake yeniden çalıştırıldığında tekrar test edilmez.
    # ==========================================================================

    check_include_file("stdio.h"   HAVE_STDIO_H)
    check_include_file("stdlib.h"  HAVE_STDLIB_H)
    check_include_file("unistd.h"  HAVE_UNISTD_H)
    check_include_file("pthread.h" HAVE_PTHREAD_H)

    if(NOT HAVE_STDIO_H)
        message(FATAL_ERROR "[SystemCheck] stdio.h not found!")
    endif()

    if(HAVE_PTHREAD_H)
        add_compile_definitions(HAVE_PTHREAD=1)
        list(APPEND CHECK_RESULTS "pthread:OK")
    else()
        list(APPEND CHECK_RESULTS "pthread:MISSING")
    endif()

    # ==========================================================================
    # CheckFunctionExists
    # Fonksiyonun link aşamasında bulunup bulunmadığını test eder.
    # CheckSymbolExists'ten farkı: sadece link zamanında arar, makroları görmez.
    # ==========================================================================

    check_function_exists(malloc HAVE_MALLOC)
    check_function_exists(popen  HAVE_POPEN)

    if(HAVE_POPEN)
        add_compile_definitions(HAVE_POPEN=1)
        list(APPEND CHECK_RESULTS "popen:OK")
    else()
        list(APPEND CHECK_RESULTS "popen:MISSING")
        message(WARNING "[SystemCheck] popen not found. MODULE_NETWORK will not work.")
    endif()

    # ==========================================================================
    # CheckSymbolExists
    # Makrolar ve inline fonksiyonları da kontrol eder.
    # CheckFunctionExists'ten daha güvenilir: sembolün hangi header'dan
    # geldiği belirtilir, makro olarak tanımlananlar da bulunur.
    # ==========================================================================

    check_symbol_exists(INT_MAX  "limits.h" HAVE_INT_MAX)
    check_symbol_exists(NULL     "stddef.h" HAVE_NULL)

    if(HAVE_INT_MAX)
        add_compile_definitions(HAVE_LIMIT_MACROS=1)
        list(APPEND CHECK_RESULTS "limits:OK")
    endif()

    # ==========================================================================
    # CheckTypeSize
    # Veri tipinin sizeof() değerini öğrenir.
    # Pointer boyutundan 32/64 bit mimari belirlenir.
    # ==========================================================================

    check_type_size("int"   SIZE_OF_INT)
    check_type_size("void*" SIZE_OF_POINTER)

    if(SIZE_OF_POINTER EQUAL 8)
        add_compile_definitions(ARCH_64BIT=1)
        list(APPEND CHECK_RESULTS "arch:64bit")
    else()
        add_compile_definitions(ARCH_32BIT=1)
        list(APPEND CHECK_RESULTS "arch:32bit")
    endif()

    message(STATUS "  int=${SIZE_OF_INT}B | ptr=${SIZE_OF_POINTER}B")

    # ==========================================================================
    # CheckCSourceCompiles
    # Verilen C kodunun başarıyla derlenip derlenmediğini test eder.
    # Link ve çalışma testi yapmaz.
    # ==========================================================================

    check_c_source_compiles("
        int test(int * restrict p) { return *p; }
        int main(void) { return 0; }
    " HAVE_C99_RESTRICT)

    check_c_source_compiles("
        void __attribute__((constructor)) init(void) {}
        int main(void) { return 0; }
    " HAVE_ATTR_CONSTRUCTOR)

    if(HAVE_C99_RESTRICT)
        add_compile_definitions(HAVE_C99_RESTRICT=1)
        list(APPEND CHECK_RESULTS "c99-restrict:OK")
    endif()

    if(HAVE_ATTR_CONSTRUCTOR)
        add_compile_definitions(HAVE_ATTR_CONSTRUCTOR=1)
        list(APPEND CHECK_RESULTS "attr-constructor:OK")
    endif()

    # ==========================================================================
    # CheckCSourceRuns
    # Verilen C kodunun derlenip çalışıp çalışmadığını test eder.
    # Cross-compile durumunda kullanılamaz.
    # ==========================================================================

    check_c_source_runs("
        int main(void) {
            unsigned int x = 1;
            return (*((char*)&x) == 1) ? 0 : 1;
        }
    " IS_LITTLE_ENDIAN)

    if(IS_LITTLE_ENDIAN)
        add_compile_definitions(SYSTEM_LITTLE_ENDIAN=1)
        list(APPEND CHECK_RESULTS "endian:little")
    else()
        add_compile_definitions(SYSTEM_BIG_ENDIAN=1)
        list(APPEND CHECK_RESULTS "endian:big")
    endif()

    # ==========================================================================
    # list komutları
    #
    # CMake listesi noktalı virgülle ayrılmış string'dir: "a;b;c"
    # ==========================================================================

    # APPEND / LENGTH / GET / FIND
    list(APPEND CHECK_RESULTS "cmake:${CMAKE_VERSION}")
    list(LENGTH CHECK_RESULTS RESULT_COUNT)
    list(GET    CHECK_RESULTS 0 FIRST_RESULT)
    list(FIND   CHECK_RESULTS "popen:OK" POPEN_IDX)    # -1 → bulunamadı

    # REMOVE_DUPLICATES: tekrar eden elemanları siler
    list(REMOVE_DUPLICATES CHECK_RESULTS)

    # SORT: alfabetik sıralar
    list(SORT CHECK_RESULTS)

    # FILTER EXCLUDE: pattern eşleşen elemanları listeden çıkarır
    # FILTER INCLUDE: sadece eşleşenleri bırakır
    set(FILTERED ${CHECK_RESULTS})
    list(FILTER FILTERED EXCLUDE REGEX ":MISSING$")    # başarısız olanları çıkar

    # TRANSFORM: her elemana dönüşüm uygular
    set(UPPER_RESULTS ${FILTERED})
    list(TRANSFORM UPPER_RESULTS TOUPPER)

    # JOIN: listeyi ayraçla tek string'e dönüştürür
    list(JOIN CHECK_RESULTS " | " RESULTS_STR)

    # ==========================================================================
    # foreach döngüleri
    # ==========================================================================

    # IN LISTS: liste elemanları üzerinde döner
    foreach(RESULT IN LISTS CHECK_RESULTS)
        message(DEBUG "[SystemCheck] ${RESULT}")
    endforeach()

    # RANGE başlangıç-bitiş-adım: sayı aralığı
    foreach(I RANGE 0 ${RESULT_COUNT} 2)
        message(DEBUG "[SystemCheck] Even index ${I}")
    endforeach()

    # ZIP_LISTS: iki listeyi paralel iter (CMake 3.17+)
    set(CHECK_KEYS   "os"         "cores"        "ram")
    set(CHECK_VALS   "${HOST_OS}" "${HOST_CORES}" "${HOST_RAM}")
    foreach(KEY VAL IN ZIP_LISTS CHECK_KEYS CHECK_VALS)
        message(DEBUG "[SystemCheck] ${KEY} = ${VAL}")
    endforeach()

    # ==========================================================================
    # Sonuç
    # ==========================================================================

    message(STATUS "  Results: ${RESULTS_STR}")

    if(SYSTEM_CHECK_STRICT AND "popen:MISSING" IN_LIST CHECK_RESULTS)
        message(FATAL_ERROR "[SystemCheck] Required checks failed.")
    endif()

    set(SYSTEM_CHECK_RESULTS ${CHECK_RESULTS} PARENT_SCOPE)
    set(HOST_IS_LITTLE_ENDIAN ${IS_LITTLE_ENDIAN} PARENT_SCOPE)

endfunction()

run_system_checks()