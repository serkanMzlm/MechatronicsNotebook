# ==============================================================================
# Git.cmake — Git Bilgisi ve CMake Komut Örnekleri
#
# Kapsanan komutlar:
#   execute_process  → dış komut çalıştırma
#   string           → STRIP, LENGTH, SUBSTRING, REPLACE, TOUPPER,
#                      FIND, CONCAT, REPEAT, REGEX MATCH, REGEX REPLACE
#   file             → MAKE_DIRECTORY, WRITE, APPEND, READ, TIMESTAMP, TO_CMAKE_PATH
#   message          → STATUS, WARNING, AUTHOR_WARNING, DEBUG, VERBOSE
# ==============================================================================

# ==============================================================================
# git_check_available — execute_process + temel if kontrolleri
# ==============================================================================
function(git_check_available)

    # COMMAND          → çalıştırılacak komut
    # RESULT_VARIABLE  → exit code (0 = başarı)
    # ERROR_QUIET      → stderr'i gizler
    # WORKING_DIRECTORY → komutun çalışacağı dizin
    execute_process(
        COMMAND git rev-parse --is-inside-work-tree
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_RESULT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(NOT GIT_RESULT EQUAL 0 OR NOT EXISTS "${CMAKE_SOURCE_DIR}/.git")
        message(WARNING "[Git] Not a Git repository. Skipping.")
        set(GIT_AVAILABLE FALSE PARENT_SCOPE)
        return()
    endif()

    set(GIT_AVAILABLE TRUE PARENT_SCOPE)
    message(STATUS "[Git] Repository found: ${CMAKE_SOURCE_DIR}")

endfunction()

# ==============================================================================
# git_get_info — execute_process + string komutları
# ==============================================================================
function(git_get_info)

    if(NOT GIT_AVAILABLE)
        return()
    endif()

    # Tek komutla birden fazla bilgi çeker — %n satır ayracıdır
    execute_process(
        COMMAND git log -1 --format=%H%n%s%n%ci%n%D
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_LOG
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # REPLACE: \n → ; dönüşümü ile CMake listesi üretir
    string(REPLACE "\n" ";" GIT_LOG_LIST "${GIT_LOG}")
    list(GET GIT_LOG_LIST 0 FULL_HASH)
    list(GET GIT_LOG_LIST 1 COMMIT_MSG)
    list(GET GIT_LOG_LIST 2 COMMIT_DATE)
    list(GET GIT_LOG_LIST 3 REFS)

    # LENGTH: string uzunluğunu alır
    string(LENGTH "${FULL_HASH}" HASH_LEN)

    # SUBSTRING: tam hash'ten 8 karakterlik kısa versiyon
    string(SUBSTRING "${FULL_HASH}" 0 8 SHORT_HASH)

    # TOUPPER: büyük harfe çevirir
    string(TOUPPER "${SHORT_HASH}" SHORT_HASH_UPPER)

    # FIND: refs içinde branch adını bulmak için konum alır
    string(FIND "${REFS}" "HEAD -> " HEAD_POS)
    if(HEAD_POS GREATER_EQUAL 0)
        string(REGEX REPLACE ".*HEAD -> ([^,\n]+).*" "\\1" BRANCH "${REFS}")
        string(STRIP "${BRANCH}" BRANCH)
    else()
        set(BRANCH "detached")
    endif()

    # CONCAT: branch + hash + tarihten build etiketi üretir
    string(SUBSTRING "${COMMIT_DATE}" 0 10 DATE_SHORT)
    string(CONCAT BUILD_LABEL "${BRANCH}" "@" "${SHORT_HASH}" "-" "${DATE_SHORT}")

    # REPEAT: rapor başlığı için ayraç satırı
    string(REPEAT "-" 40 DASH_LINE)

    # REGEX MATCH: commit mesajında semver varsa yakalar
    string(REGEX MATCH "v?([0-9]+\\.[0-9]+\\.[0-9]+)" SEMVER_MATCH "${COMMIT_MSG}")

    # REGEX REPLACE: branch adındaki özel karakterleri temizler
    string(REGEX REPLACE "[^a-zA-Z0-9_-]" "_" SAFE_BRANCH "${BRANCH}")

    set(GIT_SHORT_HASH  ${SHORT_HASH}   PARENT_SCOPE)
    set(GIT_BRANCH      ${BRANCH}       PARENT_SCOPE)
    set(GIT_SAFE_BRANCH ${SAFE_BRANCH}  PARENT_SCOPE)
    set(GIT_DATE        ${DATE_SHORT}   PARENT_SCOPE)
    set(GIT_MSG         ${COMMIT_MSG}   PARENT_SCOPE)
    set(GIT_BUILD_LABEL ${BUILD_LABEL}  PARENT_SCOPE)
    set(GIT_DASH_LINE   ${DASH_LINE}    PARENT_SCOPE)
    set(GIT_SEMVER      ${SEMVER_MATCH} PARENT_SCOPE)

endfunction()

# ==============================================================================
# git_write_report — file komutları
# ==============================================================================
function(git_write_report)

    if(NOT GIT_AVAILABLE)
        return()
    endif()

    # MAKE_DIRECTORY: dizin oluşturur (mkdir -p)
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/reports")

    # TIMESTAMP: dosyanın son değiştirilme zamanını alır
    file(TIMESTAMP "${CMAKE_CURRENT_LIST_FILE}" MTIME "%Y-%m-%d %H:%M" UTC)
    string(TIMESTAMP NOW "%Y-%m-%d %H:%M:%S UTC")

    set(REPORT "${CMAKE_BINARY_DIR}/reports/git_info.txt")

    # WRITE: dosyayı oluşturur veya üzerine yazar
    file(WRITE "${REPORT}" "# Git Report — ${NOW}\n#\n")

    # APPEND: mevcut dosyaya satır ekler
    file(APPEND "${REPORT}" "branch      = ${GIT_BRANCH}\n")
    file(APPEND "${REPORT}" "hash        = ${GIT_SHORT_HASH}\n")
    file(APPEND "${REPORT}" "date        = ${GIT_DATE}\n")
    file(APPEND "${REPORT}" "build_label = ${GIT_BUILD_LABEL}\n")

    # READ: dosya içeriğini değişkene okur
    file(READ "${REPORT}" REPORT_CONTENT)
    message(DEBUG "[Git] Report:\n${REPORT_CONTENT}")

    # TO_CMAKE_PATH: platform bağımsız yol formatına çevirir (\ → /)
    file(TO_CMAKE_PATH "${CMAKE_BINARY_DIR}/reports" REPORT_DIR)
    message(STATUS "[Git] Report → ${REPORT_DIR}/git_info.txt")

endfunction()

# ==============================================================================
# git_print_summary — message seviyeleri
# ==============================================================================
function(git_print_summary)

    if(NOT GIT_AVAILABLE)
        message(WARNING "[Git] No summary — git info not available.")
        return()
    endif()

    # STATUS   : standart bilgi çıktısı
    # DEBUG    : sadece --log-level=DEBUG ile görünür
    # VERBOSE  : sadece --log-level=VERBOSE ile görünür
    # AUTHOR_WARNING: proje geliştiricisine yönelik uyarı
    message(STATUS "${GIT_DASH_LINE}")
    message(STATUS "  Branch : ${GIT_BRANCH}")
    message(STATUS "  Hash   : ${GIT_SHORT_HASH}")
    message(STATUS "  Label  : ${GIT_BUILD_LABEL}")
    message(STATUS "${GIT_DASH_LINE}")
    message(DEBUG   "[Git] Safe branch : ${GIT_SAFE_BRANCH}")
    message(VERBOSE "[Git] Semver match: ${GIT_SEMVER}")

    if(GIT_BRANCH MATCHES "^(feature|fix)/" AND CMAKE_BUILD_TYPE STREQUAL "Release")
        message(AUTHOR_WARNING "[Git] Release build on non-release branch: ${GIT_BRANCH}")
    endif()

endfunction()

# ==============================================================================
# Çağrılar — include(Git) yapıldığında otomatik çalışır
# ==============================================================================
# git_check_available()
git_get_info()
git_write_report()
# git_print_summary()