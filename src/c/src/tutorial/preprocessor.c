/*
 * preprocessor.c — C Önişlemci Örnekleri
 *
 * Kapsanan konular:
 *   #define / #undef        → sabit ve makro tanımlama/silme
 *   #if / #ifdef / #ifndef  → koşullu derleme
 *   #pragma                 → derleyici direktifleri
 *   Stringify (#)           → argümanı string'e çevirme
 *   Token paste (##)        → iki token'ı birleştirme
 *   X-macro                 → kod üretimi pattern
 *   __FILE__ / __LINE__     → yerleşik makrolar
 */

#include <stdio.h>
#include <stdint.h>

/* ===========================================================================
 * #pragma once — çift include koruması (common.h'da kullanıldı)
 * #pragma pack — struct padding kontrolü
 * #pragma GCC optimize — fonksiyon bazlı optimizasyon
 * ===========================================================================*/
#pragma GCC optimize("O2")

/* ===========================================================================
 * #define / #undef
 *
 * Object-like : parametre almaz
 * Function-like: parametre alır, parantez zorunlu
 * ===========================================================================*/
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#define VERSION_STR \
    STRINGIFY(VERSION_MAJOR) "." \
    STRINGIFY(VERSION_MINOR) "." \
    STRINGIFY(VERSION_PATCH)

/* Stringify: # operatörü argümanı string literal'e çevirir */
#define STRINGIFY(x)  #x
#define TOSTRING(x)   STRINGIFY(x)   /* iki katman: makro expand için */

/* Token paste: ## iki token'ı birleştirir */
#define MAKE_VAR(prefix, num)  prefix##num
#define MAKE_FN(name)          fn_##name

/* Güvenli function-like makro — do{}while(0) pattern */
#define SWAP(type, a, b)  \
    do {                  \
        type _t = (a);    \
        (a) = (b);        \
        (b) = _t;         \
    } while(0)

/* ===========================================================================
 * Koşullu derleme
 *
 * #ifdef  : tanımlıysa
 * #ifndef : tanımlı değilse
 * #if     : sayısal koşul
 * #elif   : else if
 * #else   : aksi halde
 * #endif  : blok sonu
 * ===========================================================================*/

/* Platform tespiti */
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM "Windows"
#elif defined(__linux__)
    #define PLATFORM "Linux"
#elif defined(__APPLE__)
    #define PLATFORM "macOS"
#else
    #define PLATFORM "Unknown"
#endif

/* Derleyici tespiti */
#if defined(__GNUC__) && !defined(__clang__)
    #define COMPILER "GCC " TOSTRING(__GNUC__)
#elif defined(__clang__)
    #define COMPILER "Clang " TOSTRING(__clang_major__)
#elif defined(_MSC_VER)
    #define COMPILER "MSVC"
#else
    #define COMPILER "Unknown"
#endif

/* Debug makroları — sadece DEBUG tanımlıysa aktif */
#ifdef DEBUG
    #define DBG(fmt, ...) \
        fprintf(stderr, "[DBG %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define DBG(fmt, ...)   /* boş — release'de sıfır maliyet */
#endif

/* ===========================================================================
 * Yerleşik makrolar (predefined)
 *
 * __FILE__       → mevcut dosya adı (string)
 * __LINE__       → mevcut satır numarası (int)
 * __func__       → mevcut fonksiyon adı (C99)
 * __DATE__       → derleme tarihi (string)
 * __TIME__       → derleme saati (string)
 * __STDC_VERSION__ → C standardı (C99=199901L, C11=201112L)
 * ===========================================================================*/
#define HERE()  printf("  %s:%d — %s()\n", __FILE__, __LINE__, __func__)

#define ASSERT(cond)                                          \
    do {                                                      \
        if (!(cond))                                          \
            fprintf(stderr, "ASSERT FAILED: %s\n"            \
                    "  at %s:%d in %s()\n",                   \
                    #cond, __FILE__, __LINE__, __func__);     \
    } while(0)

/* ===========================================================================
 * X-macro pattern
 *
 * Tek bir liste tanımlanır, farklı bağlamlarda farklı şekilde expand edilir.
 * Enum, string tablosu, switch case'i senkron tutar.
 * ===========================================================================*/
#define ERROR_LIST(X)           \
    X(ERR_NONE,    "No error")  \
    X(ERR_INVALID, "Invalid argument") \
    X(ERR_NOMEM,   "Out of memory")    \
    X(ERR_IO,      "I/O error")        \
    X(ERR_TIMEOUT, "Timeout")

/* Enum üret */
typedef enum
{
#define X_ENUM(code, msg) code,
    ERROR_LIST(X_ENUM)
#undef X_ENUM
    ERR_COUNT
} ErrorCode_t;

/* String tablosu üret */
static const char *error_messages[] =
{
#define X_MSG(code, msg) msg,
    ERROR_LIST(X_MSG)
#undef X_MSG
};

static const char *error_to_string(ErrorCode_t code)
{
    if (code >= ERR_COUNT) return "Unknown";
    return error_messages[code];
}

/* ===========================================================================
 * #pragma pack — struct padding kontrolü
 * ===========================================================================*/
#pragma pack(push, 1)   /* 1-byte hizalama */
typedef struct
{
    uint8_t  type;
    uint16_t length;
    uint32_t id;
    uint8_t  flags;
} PackedHeader_t;
#pragma pack(pop)       /* önceki hizalamaya dön */

typedef struct
{
    uint8_t  type;
    uint16_t length;
    uint32_t id;
    uint8_t  flags;
} NormalHeader_t;

/* ===========================================================================
 * main
 * ===========================================================================*/
int main(void)
{
    /* Versiyon ve platform bilgisi */
    printf("Version  : %s\n", VERSION_STR);
    printf("Platform : %s\n", PLATFORM);
    printf("Compiler : %s\n", COMPILER);
    printf("Built    : %s %s\n", __DATE__, __TIME__);

#if __STDC_VERSION__ >= 201112L
    printf("Standard : C11\n");
#elif __STDC_VERSION__ >= 199901L
    printf("Standard : C99\n");
#else
    printf("Standard : C89\n");
#endif

    printf("\n");

    /* Yerleşik makrolar */
    printf("Location:\n");
    HERE();

    /* ASSERT */
    ASSERT(1 + 1 == 2);          /* geçer */
    ASSERT(sizeof(int) >= 2);    /* geçer */

    /* DBG — sadece -DDEBUG ile derlendiyse çıktı üretir */
    DBG("debug message: %d", 42);

    printf("\n");

    /* Token paste */
    int MAKE_VAR(val, 1) = 10;
    int MAKE_VAR(val, 2) = 20;
    printf("token paste: val1=%d val2=%d\n", val1, val2);

    /* Stringify */
    printf("stringify: %s\n", STRINGIFY(Hello World));
    printf("tostring:  %s\n", TOSTRING(VERSION_MAJOR));

    /* SWAP makrosu */
    int a = 5, b = 9;
    SWAP(int, a, b);
    printf("swap: a=%d b=%d\n", a, b);

    printf("\n");

    /* X-macro: error tablosu */
    printf("Error codes:\n");
    for (int i = 0; i < ERR_COUNT; i++)
        printf("  [%d] %s\n", i, error_to_string((ErrorCode_t)i));

    printf("\n");

    /* #pragma pack etkisi */
    printf("sizeof PackedHeader_t : %zu (packed)\n", sizeof(PackedHeader_t));
    printf("sizeof NormalHeader_t : %zu (with padding)\n", sizeof(NormalHeader_t));

    return 0;
}