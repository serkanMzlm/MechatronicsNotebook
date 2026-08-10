#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* ===========================================================================
 * common.h — Ortak Tanımlar, Makrolar ve Tipler
 *
 * Kapsanan konular:
 *   Makrolar          → object-like, function-like, variadic, güvenli yazım
 *   static inline     → makro yerine güvenli tip-güvenli alternatif
 *   _Static_assert    → derleme zamanı kontrol
 *   __attribute__     → constructor, destructor, unused, packed, noreturn
 *   typedef enum      → enum tanımı ve kullanımı
 *   typedef struct    → struct tanımı, bitfield
 *   typedef union     → union ve memory overlay
 * ===========================================================================*/

/* ===========================================================================
 * BÖLÜM 1 — Terminal renk kodları (ANSI escape sequences)
 *
 * Object-like macro: parametre almaz, sabit bir değere expand olur.
 * \x1b[ → ESC karakteri (ASCII 27), ANSI terminal kontrolü için.
 * ===========================================================================*/

#define COLOR_RST  "\x1b[0m"
#define COLOR_RED  "\x1b[31m"
#define COLOR_GRN  "\x1b[32m"
#define COLOR_YLW  "\x1b[33m"
#define COLOR_BLE  "\x1b[34m"
#define COLOR_MGT  "\x1b[35m"
#define COLOR_CYN  "\x1b[36m"
#define COLOR_WHT  "\x1b[37m"

#define LINE "================================================\n"

/* ===========================================================================
 * BÖLÜM 2 — Log makroları
 *
 * Variadic macro: __VA_ARGS__ ile değişken sayıda argüman alır.
 * do { } while(0) pattern: makronun her zaman tek deyim gibi davranmasını sağlar.
 *   Olmadan: if(x) LOG(...) else ... yapısı bozulabilir.
 *
 * ## operatörü: __VA_ARGS__ boşsa önceki virgülü siler (GCC/Clang uzantısı).
 * ===========================================================================*/

#define LOG_ERROR(fmt, ...) \
    do { printf(COLOR_RED "[ERROR] " fmt COLOR_RST "\n", ##__VA_ARGS__); } while(0)

#define LOG_WARN(fmt, ...) \
    do { printf(COLOR_YLW "[WARN]  " fmt COLOR_RST "\n", ##__VA_ARGS__); } while(0)

#define LOG_INFO(fmt, ...) \
    do { printf(COLOR_WHT "[INFO]  " fmt COLOR_RST "\n", ##__VA_ARGS__); } while(0)

#define LOG_DEBUG(fmt, ...) \
    do { printf(COLOR_GRN "[DEBUG] " fmt COLOR_RST "\n", ##__VA_ARGS__); } while(0)

/* ===========================================================================
 * BÖLÜM 3 — Yardımcı makrolar
 *
 * Hatalı yazım örnekleri ve düzeltmeleri:
 *
 *   KÖTÜ → #define SQUARE(x) x*x
 *           SQUARE(1+2) → 1+2*1+2 = 5  (beklenmedik sonuç)
 *
 *   İYİ  → #define SQUARE(x) ((x)*(x))
 *           SQUARE(1+2) → ((1+2)*(1+2)) = 9
 *
 *   KÖTÜ → #define MAX(a,b) (a) < (b) ? (b) : (a)
 *           x = MAX(a,b) + 1 → yanlış öncelik
 *
 *   İYİ  → tüm ifade parantez içinde
 *
 * NOT: Yan etkili ifadelerle makro hâlâ tehlikelidir:
 *   SQUARE(i++) → ((i++)*(i++))  — tanımsız davranış (undefined behavior)
 *   Bu yüzden tip-güvenli inline fonksiyon tercih edilmeli (Bölüm 4).
 * ===========================================================================*/

#define SQUARE(x)      ((x) * (x))
#define MAX(x, y)      (((x) > (y)) ? (x) : (y))
#define MIN(x, y)      (((x) < (y)) ? (x) : (y))
#define CLAMP(v, lo, hi) (MAX((lo), MIN((v), (hi))))
#define ABS(x)         (((x) < 0) ? -(x) : (x))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* Stringification: # operatörü argümanı string literaline dönüştürür */
#define STRINGIFY(x)   #x
#define TOSTRING(x)    STRINGIFY(x)

/* Token pasting: ## operatörü iki token'ı birleştirir */
#define MAKE_NAME(prefix, suffix) prefix##_##suffix

/* ===========================================================================
 * BÖLÜM 4 — static inline fonksiyonlar
 *
 * Makro yerine tercih edilmeli çünkü:
 *   - Tip güvenlidir (type-safe)
 *   - Yan etkili argümanlarda güvenlidir (i++ gibi)
 *   - Hata ayıklanabilir (debugger görebilir)
 *   - Kapsam (scope) kurallarına uyar
 *
 * static: her çeviri biriminde (translation unit) ayrı kopya — bağlantı sorunu olmaz.
 * inline: derleyiciye "bu çağrıyı satır içine al" isteği (garanti değil).
 * ===========================================================================*/

static inline int square_safe(int x)
{
    return x * x;
}

static inline int max_safe(int a, int b)
{
    return (a > b) ? a : b;
}

static inline int clamp_safe(int val, int lo, int hi)
{
    return (val < lo) ? lo : (val > hi) ? hi : val;
}

/* Bit işlemleri için inline yardımcılar */
static inline bool bit_is_set(uint32_t reg, uint8_t bit)
{
    return (reg & (1u << bit)) != 0;
}

static inline uint32_t bit_set(uint32_t reg, uint8_t bit)
{
    return reg | (1u << bit);
}

static inline uint32_t bit_clear(uint32_t reg, uint8_t bit)
{
    return reg & ~(1u << bit);
}

/* ===========================================================================
 * BÖLÜM 5 — _Static_assert
 *
 * Derleme zamanında koşul kontrolü yapar — runtime maliyeti sıfır.
 * C11 ile gelmiştir. C23'te assert() gibi tek argümanla da kullanılabilir.
 *
 * Kullanım yerleri:
 *   - Struct boyutu garantisi (protokol / donanım uyumu)
 *   - Tip boyutu varsayımları
 *   - Derleme zamanı sabiti kontrolü
 * ===========================================================================*/

_Static_assert(sizeof(int)     >= 2, "int must be at least 2 bytes");
_Static_assert(sizeof(int32_t) == 4, "int32_t must be exactly 4 bytes");
_Static_assert(sizeof(void *)  >= 4, "pointer must be at least 4 bytes");

/* ===========================================================================
 * BÖLÜM 6 — __attribute__ (GCC / Clang)
 *
 * Derleyiciye ek bilgi verir; optimizasyon, uyarı ve davranış kontrolü sağlar.
 *
 * constructor  → main() çalışmadan önce otomatik çağrılır
 * destructor   → main() bittikten sonra otomatik çağrılır
 * unused       → kullanılmayan değişken/fonksiyon uyarısını bastırır
 * packed       → struct padding'i kaldırır (donanım register'ları için)
 * noreturn     → fonksiyonun geri dönmeyeceğini bildirir (exit, abort gibi)
 * deprecated   → kullanımda uyarı verir
 * ===========================================================================*/

/* noreturn: derleyici bu fonksiyondan sonra kod üretmez */
__attribute__((noreturn))
static inline void fatal_error(const char *msg)
{
    fprintf(stderr, COLOR_RED "[FATAL] %s" COLOR_RST "\n", msg);
    __builtin_unreachable(); /* UB olmadan derleyiciye ipucu */
}

/* deprecated: bu fonksiyonu kullanan kod derleme uyarısı alır */
__attribute__((deprecated("use LOG_ERROR instead")))
static inline void old_log(const char *msg)
{
    printf("[LOG] %s\n", msg);
}

/* ===========================================================================
 * BÖLÜM 7 — typedef enum
 *
 * Enum: derleme zamanı sabitleri — magic number kullanımını önler.
 * typedef: isimsiz enum'u doğrudan tip olarak kullanmayı sağlar.
 *
 * İyi pratik:
 *   - Son eleman COUNT/MAX gibi bir isimle toplam sayıyı tutar
 *   - ARRAY_SIZE yerine bu sayaç kullanılabilir
 * ===========================================================================*/

typedef enum
{
    STATE_X = 0,
    STATE_Y,
    STATE_Z,
    STATE_COUNT   /* toplam eleman sayısı — dizi boyutu için kullanılır */
} StateIndex_t;

typedef enum
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_COUNT
} LogLevel_t;

/* ===========================================================================
 * BÖLÜM 8 — typedef struct
 *
 * Struct: farklı tipteki verileri bir arada tutar.
 *
 * Bitfield: struct üyelerinin bit seviyesinde boyutunu belirler.
 *   - Donanım register map'leri için yaygın kullanılır
 *   - Derleyici padding ekleyebilir; __attribute__((packed)) ile önlenir
 *   - Taşınabilirlik: bit sırası (endianness) platforma bağlıdır
 * ===========================================================================*/

typedef struct
{
    char  name[20];
    int   age;
    float weight;
} People_t;

/* Bitfield örneği — donanım register simülasyonu */
typedef struct __attribute__((packed))
{
    uint8_t power   : 1;   /* bit 0 — sadece 0 veya 1 */
    uint8_t mode    : 2;   /* bit 1-2 — 0-3 arası değer */
    uint8_t channel : 4;   /* bit 3-6 — 0-15 arası değer */
    uint8_t error   : 1;   /* bit 7 */
} DeviceConfig_t;

_Static_assert(sizeof(DeviceConfig_t) == 1, "DeviceConfig_t must be 1 byte");

/* ===========================================================================
 * BÖLÜM 9 — typedef union
 *
 * Union: tüm üyeler aynı memory bölgesini paylaşır.
 * Boyutu en büyük üyenin boyutuna eşittir.
 *
 * Kullanım alanları:
 *   - Tip dönüşümü (type punning) — dikkatli kullanılmalı
 *   - Memory overlay (aynı veriye farklı görünümler)
 *   - Protokol parsing (byte/word/dword erişimi)
 * ===========================================================================*/

typedef union
{
    struct
    {
        int x;
        int y;
        int z;
    };
    int state[STATE_COUNT];   /* aynı memory'ye dizi olarak erişim */
} State_u;

/* Tip dönüşümü için union — float'ın bit pattern'ini okur */
typedef union
{
    float    f;
    uint32_t u;
    uint8_t  bytes[4];
} FloatBits_t;

/* ===========================================================================
 * BÖLÜM 10 — Fonksiyon prototipleri (constructor/destructor örnekleri)
 *
 * Prototip bildirimi: başlık dosyasında tanım değil, sadece imza.
 * Derleyici bu imzayı kullanarak tip kontrolü yapar.
 * ===========================================================================*/

/* __attribute__((constructor)): main() öncesi çalışır, öncelik verilebilir */
void __attribute__((constructor(101))) system_init(void);

/* __attribute__((destructor)): main() sonrası çalışır */
void __attribute__((destructor(101)))  system_cleanup(void);

#endif /* COMMON_H */