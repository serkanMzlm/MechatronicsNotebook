/*
 * basics.c — Temel C Kavramları
 *
 * Kapsanan konular:
 *   Veri tipleri      → tam sayı, ondalık, char, bool
 *   signed/unsigned   → fark, taşma davranışı
 *   const             → değişmez değer, pointer kombinasyonları
 *   static            → yerel static, dosya kapsamı static
 *   volatile          → derleyici optimizasyonunu önleme
 *   register          → öneri niteliğinde hız ipucu
 *   Type casting      → implicit, explicit, tehlikeli dönüşümler
 *   sizeof / alignof  → boyut ve bellek hizalama
 *   Limitler          → limits.h ve float.h sabitleri
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <stddef.h>   /* offsetof, size_t */

#include "common.h"

/* ===========================================================================
 * Dosya kapsamı static değişken
 *
 * static — dosya dışından erişilemez (internal linkage).
 * Global değişkenden farkı: bu sembol başka .c dosyalarında görünmez.
 * Production kodunda global state yerine bu tercih edilir.
 * ===========================================================================*/
static int file_scope_counter = 0;

/* ===========================================================================
 * BÖLÜM 1 — Temel veri tipleri
 *
 * C standardı minimum boyutları garanti eder, tam boyutu değil.
 *   char       ≥ 8 bit
 *   short      ≥ 16 bit
 *   int        ≥ 16 bit (pratikte 32 bit)
 *   long       ≥ 32 bit
 *   long long  ≥ 64 bit
 *
 * Kesin boyut için <stdint.h> kullan: int8_t, uint16_t, int32_t ...
 * ===========================================================================*/
static void demo_data_types(void)
{
    /* Temel tipler ve boyutları */
    char      c  = 'A';
    short     s  = 32767;
    int       i  = -2147483648;
    long      l  = 123456789L;
    long long ll = 9223372036854775807LL;

    /* Ondalık tipler */
    float       f  = 3.14f;     /* f soneki: float literal */
    double      d  = 3.141592;
    long double ld = 3.14159265358979L;

    /* bool — <stdbool.h> ile gelir, C99+ */
    bool flag = true;

    /* Sabit boyutlu tipler — taşınabilir kod için tercih edilmeli */
    int8_t   i8  = -128;
    uint8_t  u8  = 255;
    int16_t  i16 = -32768;
    uint32_t u32 = 4294967295U;
    int64_t  i64 = -9223372036854775807LL;

    printf("--- Data Types ---\n");
    printf("char:      %c  (%d)\n",  c, c);
    printf("short:     %d\n",  s);
    printf("int:       %d\n",  i);
    printf("long:      %ld\n", l);
    printf("long long: %lld\n", ll);
    printf("float:     %.6f\n", f);
    printf("double:    %.15f\n", d);
    printf("long dbl:  %.18Lf\n", ld);
    printf("bool:      %s\n", flag ? "true" : "false");
    printf("int8_t:    %d\n",  i8);
    printf("uint8_t:   %u\n",  u8);
    printf("uint32_t:  %u\n",  u32);
    printf("int64_t:   %lld\n", (long long)i64);
    printf("int16_t:   %d\n",  i16);
}

/* ===========================================================================
 * BÖLÜM 2 — signed / unsigned farkı
 *
 * signed:   negatif ve pozitif değer taşır, taşmada UB (undefined behavior)
 * unsigned: sadece pozitif, taşmada modüler aritmetik (well-defined)
 *
 * Dikkat:
 *   - signed overflow → undefined behavior (derleyici optimize edebilir)
 *   - unsigned overflow → modulo 2^N (güvenli, well-defined)
 *   - signed ile unsigned karşılaştırma: signed, unsigned'a convert edilir
 *     Bu dönüşüm beklenmedik sonuçlar üretebilir.
 * ===========================================================================*/
static void demo_signed_unsigned(void)
{
    unsigned int u = 0;
    int          s = -1;

    /* Tehlikeli karşılaştırma: -1 unsigned'a dönüşünce çok büyük sayı olur */
    if ((unsigned int)s > u)
        printf("WARNING: -1 > 0 as unsigned!\n");

    /* unsigned taşma — well-defined, modulo 2^32 */
    uint8_t byte = 255;
    byte++;                        /* 255 + 1 = 0 (wrap-around) */
    printf("uint8 wrap: %u\n", byte);

    /* unsigned ile bit maskeleme — güvenli yol */
    uint32_t flags = 0xDEADBEEF;
    uint8_t  low   = (uint8_t)(flags & 0xFF);   /* alt byte */
    printf("Low byte: 0x%02X\n", low);
}

/* ===========================================================================
 * BÖLÜM 3 — const
 *
 * const değişken: değeri değiştirilemez, derleyici korur.
 * const pointer kombinasyonları — okuma sağdan sola:
 *
 *   const int *p      → pointer to const int     (değer sabit, adres değişebilir)
 *   int *const p      → const pointer to int     (adres sabit, değer değişebilir)
 *   const int *const p → const pointer to const int (ikisi de sabit)
 * ===========================================================================*/
static void demo_const(void)
{
    const int MAX_SIZE = 100;
    /* MAX_SIZE = 200; */ /* derleme hatası */

    int x = 10;
    int y = 20;

    /* pointer to const int — değeri değiştiremeyiz, adresi değiştirebiliriz */
    const int *ptr1 = &x;
    ptr1 = &y;             /* OK — adres değişti */
    /* *ptr1 = 99; */      /* hata — değer const */

    /* const pointer to int — adres sabit, değer değişebilir */
    int *const ptr2 = &x;
    *ptr2 = 99;            /* OK — değer değişti */
    /* ptr2 = &y; */       /* hata — adres const */

    /* const pointer to const int — ikisi de sabit */
    const int *const ptr3 = &x;
    /* *ptr3 = 1; */       /* hata */
    /* ptr3 = &y; */       /* hata */

    printf("const demo: MAX=%d x=%d ptr3=%d\n", MAX_SIZE, x, *ptr3);
}

/* ===========================================================================
 * BÖLÜM 4 — static
 *
 * 1. Yerel static değişken:
 *    - Fonksiyon çağrıları arasında değerini korur
 *    - Stack'te değil, data segment'te saklanır
 *    - İlk çağrıda bir kez initialize edilir
 *
 * 2. Dosya kapsamı static (yukarıda tanımlı):
 *    - Diğer .c dosyalarından erişilemez (internal linkage)
 *
 * 3. static fonksiyon:
 *    - Sadece bu .c dosyasında görünür
 * ===========================================================================*/
static void demo_static(void)
{
    /* Yerel static: her çağrıda değerini korur */
    static int call_count = 0;
    call_count++;

    /* Dosya kapsamı static */
    file_scope_counter++;

    printf("call_count=%d | file_scope_counter=%d\n",
           call_count, file_scope_counter);
}

/* ===========================================================================
 * BÖLÜM 5 — volatile
 *
 * volatile: derleyiciye "bu değişkeni optimize etme, her okumada bellekten al"
 * der.
 *
 * Kullanım yerleri:
 *   - Donanım register'ları (memory-mapped I/O)
 *   - Interrupt handler ile paylaşılan değişkenler
 *   - setjmp/longjmp kullanan kod
 *
 * volatile tek başına thread safety sağlamaz — mutex veya atomik gerekir.
 * ===========================================================================*/

/* Interrupt handler'dan yazılan bayrak simülasyonu */
static volatile bool interrupt_flag = false;

static void demo_volatile(void)
{
    /*
     * volatile olmadan derleyici bu döngüyü optimize edip kaldırabilir:
     *   while(!flag) {}  →  derleyici flag'in değişmeyeceğini varsayar
     *
     * volatile ile: her iterasyonda flag bellekten okunur.
     */
    volatile int sensor_reg = 42;   /* donanım register simülasyonu */
    int value = sensor_reg;         /* her okumada register'dan alır */

    printf("volatile sensor: %d | interrupt_flag: %d\n",
           value, interrupt_flag);
}

/* ===========================================================================
 * BÖLÜM 6 — register (C17 ve öncesi)
 *
 * Derleyiciye "bu değişkeni CPU register'ında tut" önerisi verir.
 * Modern derleyiciler bu öneriyi büyük ölçüde görmezden gelir;
 * kendi optimizasyon kararlarını alır.
 *
 * C++17'de kaldırılmıştır. C'de hâlâ geçerli ama gereksiz sayılır.
 * Tek kısıtı: register değişkeninin adresi alınamaz (&reg_var → hata).
 * ===========================================================================*/
static void demo_register(void)
{
    register int i;
    int sum = 0;

    for (i = 0; i < 100; i++)
        sum += i;

    printf("register loop sum: %d\n", sum);
    /* printf("%p", &i); */ /* hata: register değişkenin adresi alınamaz */
}

/* ===========================================================================
 * BÖLÜM 7 — Type casting
 *
 * Implicit cast: derleyici otomatik dönüştürür (veri kaybı uyarısı verebilir)
 * Explicit cast: programcı açıkça belirtir (sorumluluğu üstlenir)
 *
 * Tehlikeli dönüşümler:
 *   - Büyük tip → küçük tip: veri kaybı
 *   - signed → unsigned: negatif değerler büyük pozitif olur
 *   - float → int: kesme (truncation), yuvarlama değil
 *   - pointer cast: alignment hatası riski
 * ===========================================================================*/
static void demo_casting(void)
{
    /* Implicit cast — derleyici otomatik genişletir */
    int    i = 42;
    double d = i;        /* int → double, veri kaybı yok */
    printf("implicit: int %d → double %.1f\n", i, d);

    /* Explicit cast — kesme (truncation) */
    double pi    = 3.99;
    int    pi_int = (int)pi;   /* 3 — yuvarlama değil, kesme */
    printf("explicit: double %.2f → int %d (truncated)\n", pi, pi_int);

    /* Tehlikeli: veri kaybı */
    int    big  = 300;
    int8_t byte = (int8_t)big;   /* 300 mod 256 = 44 */
    printf("narrowing: int %d → int8_t %d (data loss)\n", big, byte);

    /* void pointer cast — generic bellek işlemleri için */
    int   val = 100;
    void *vptr = &val;
    int  *iptr = (int *)vptr;   /* void* → int* */
    printf("void cast: %d\n", *iptr);

    /* float bit pattern okuma — union ile güvenli type punning */
    FloatBits_t fb;
    fb.f = 1.0f;
    printf("float 1.0 bits: 0x%08X\n", fb.u);
}

/* ===========================================================================
 * BÖLÜM 8 — sizeof ve alignof
 *
 * sizeof: derleme zamanında boyutu verir (byte cinsinden), runtime maliyeti sıfır.
 * alignof: tipin bellek hizalama gereksinimini verir (C11, <stdalign.h>).
 * offsetof: struct üyesinin struct başından uzaklığı (<stddef.h>).
 *
 * sizeof dizi ile pointer farkı:
 *   int arr[10]; sizeof(arr) = 40   (dizi boyutu)
 *   int *ptr = arr; sizeof(ptr) = 8 (pointer boyutu, 64-bit)
 * ===========================================================================*/
static void demo_sizeof(void)
{
    printf("--- sizeof ---\n");
    printf("char:       %zu\n", sizeof(char));
    printf("short:      %zu\n", sizeof(short));
    printf("int:        %zu\n", sizeof(int));
    printf("long:       %zu\n", sizeof(long));
    printf("long long:  %zu\n", sizeof(long long));
    printf("float:      %zu\n", sizeof(float));
    printf("double:     %zu\n", sizeof(double));
    printf("void*:      %zu\n", sizeof(void *));
    printf("People_t:   %zu\n", sizeof(People_t));

    /* Dizi vs pointer farkı */
    int arr[10];
    int *ptr = arr;
    printf("arr[10]:    %zu\n", sizeof(arr));    /* 40 */
    printf("ptr:        %zu\n", sizeof(ptr));    /* 8 (64-bit) */
    printf("arr eleman: %zu\n", ARRAY_SIZE(arr));/* 10 */

    /* offsetof: struct içi padding'i anlamak için */
    printf("--- offsetof People_t ---\n");
    printf("name:   %zu\n", offsetof(People_t, name));
    printf("age:    %zu\n", offsetof(People_t, age));
    printf("weight: %zu\n", offsetof(People_t, weight));
}

/* ===========================================================================
 * BÖLÜM 9 — Limitler
 *
 * <limits.h>: tam sayı tip sınırları
 * <float.h>:  ondalık tip sınırları ve hassasiyet bilgisi
 * ===========================================================================*/
static void demo_limits(void)
{
    printf("--- Integer Limits ---\n");
    printf("INT8  : [%d, %d]\n",   INT8_MIN,   INT8_MAX);
    printf("INT16 : [%d, %d]\n",   INT16_MIN,  INT16_MAX);
    printf("INT32 : [%d, %d]\n",   INT32_MIN,  INT32_MAX);
    printf("INT64 : [%lld, %lld]\n", INT64_MIN, INT64_MAX);
    printf("UINT8 : [0, %u]\n",    UINT8_MAX);
    printf("UINT32: [0, %u]\n",    UINT32_MAX);

    printf("--- Float Limits ---\n");
    printf("FLT_MIN:  %e\n", FLT_MIN);
    printf("FLT_MAX:  %e\n", FLT_MAX);
    printf("FLT_DIG:  %d  (decimal digits of precision)\n", FLT_DIG);
    printf("DBL_MIN:  %e\n", DBL_MIN);
    printf("DBL_MAX:  %e\n", DBL_MAX);
    printf("DBL_DIG:  %d\n", DBL_DIG);
}

/* ===========================================================================
 * constructor / destructor implementasyonları
 * Prototipleri common.h'da bildirildi.
 * ===========================================================================*/

void __attribute__((constructor(101))) system_init(void)
{
    LOG_INFO("System initialized");
}

void __attribute__((destructor(101))) system_cleanup(void)
{
    LOG_INFO("System cleanup done");
}

/* ===========================================================================
 * main
 * ===========================================================================*/

int main(void)
{
    printf(LINE);
    LOG_INFO("basics.c — starting demos");
    printf(LINE);

    demo_data_types();      printf(LINE);
    demo_signed_unsigned(); printf(LINE);
    demo_const();           printf(LINE);

    /* static demo: iki kez çağırarak davranışı göster */
    demo_static();
    demo_static();          printf(LINE);

    demo_volatile();        printf(LINE);
    demo_register();        printf(LINE);
    demo_casting();         printf(LINE);
    demo_sizeof();          printf(LINE);
    demo_limits();          printf(LINE);

    LOG_INFO("basics.c — all demos complete");

    return 0;
}