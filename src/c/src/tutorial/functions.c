/*
 * functions.c — Fonksiyon Kavramları
 *
 * Kapsanan konular:
 *   Parametre geçirme → by value, by reference, by pointer
 *   inline            → derleyici ipucu, static inline farkı
 *   variadic          → va_list, va_start, va_arg, va_end
 *   recursive         → doğrusal, kuyruk, yığın taşması riski
 *   callback          → function pointer parametre
 *   array parametre   → decay, boyut geçirme yöntemleri
 *   return struct     → değer döndürme, compound literal
 *   error handling    → errno, return code, out-parameter pattern
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "common.h"

/* ===========================================================================
 * BÖLÜM 1 — Parametre geçirme
 *
 * By value:     kopya geçilir — orijinal değişmez
 * By pointer:   adres geçilir — orijinal değiştirilebilir
 * By const ptr: adres geçilir — okuma amaçlı, değiştirilemez (güvenli)
 *
 * C'de "by reference" yoktur — pointer ile simüle edilir.
 * Büyük struct'lar değer olarak geçilirse kopyalama maliyeti artar;
 * const pointer tercih edilmeli.
 * ===========================================================================*/

static void by_value(int x)
{
    x = 999;   /* sadece yerel kopya değişir */
}

static void by_pointer(int *x)
{
    *x = 999;  /* orijinal değişir */
}

static void by_const_ptr(const People_t *p)
{
    /* p->age = 0; */ /* derleme hatası — const koruma */
    printf("read-only: %s age=%d\n", p->name, p->age);
}

static void demo_parameter_passing(void)
{
    int val = 42;

    by_value(val);
    printf("after by_value:   %d (unchanged)\n", val);

    by_pointer(&val);
    printf("after by_pointer: %d (changed)\n", val);

    People_t person = { .name = "Ali", .age = 30, .weight = 70.0f };
    by_const_ptr(&person);
}

/* ===========================================================================
 * BÖLÜM 2 — inline
 *
 * inline: derleyiciye "bu fonksiyonu çağrı yerine satır içine al" önerisi.
 * Garanti değil — derleyici reddedebilir (örn. özyinelemeli fonksiyonlar).
 *
 * static inline: her translation unit'te görünür, bağlantı sorunu olmaz.
 *   En yaygın ve güvenli kullanım biçimi.
 *
 * extern inline (C99): tek tanım kuralı (ODR) ile dikkatli kullanılmalı.
 *
 * Makro ile farkı:
 *   - Tip güvenli
 *   - Yan etkili argümanlarda güvenli (i++ gibi)
 *   - Hata ayıklanabilir
 * ===========================================================================*/

/* static inline: başlıkta tanımlanabilir, her .c dosyasına özel kopya */
static inline int fast_abs(int x)
{
    return (x < 0) ? -x : x;
}

static inline int fast_max(int a, int b)
{
    return (a > b) ? a : b;
}

static void demo_inline(void)
{
    printf("fast_abs(-7)    = %d\n", fast_abs(-7));
    printf("fast_max(3, 8)  = %d\n", fast_max(3, 8));

    /* Makro ile karşılaştırma — yan etki farkı */
    int i = 3;
    printf("SQUARE(i++)     = %d (i=%d) ← makro tehlikeli\n",
           SQUARE(i++), i);   /* (3++)*(3++) — UB */

    i = 3;
    printf("fast_abs ile  i = %d\n", fast_abs(i++));  /* güvenli */
}

/* ===========================================================================
 * BÖLÜM 3 — Variadic fonksiyon
 *
 * Değişken sayıda argüman alan fonksiyon.
 * En az bir sabit parametre zorunlu (va_start için referans).
 *
 * va_start(ap, last)  → ap'yi hazırla, last son sabit parametre
 * va_arg(ap, type)    → sıradaki argümanı type olarak oku
 * va_end(ap)          → temizlik (zorunlu)
 * va_copy(dst, src)   → ap'nin kopyasını al (ikinci geçiş için)
 *
 * Tip güvensizdir: yanlış type geçilirse undefined behavior.
 * ===========================================================================*/

/* Tamsayı toplamı — count: argüman sayısı */
static int sum_variadic(int count, ...)
{
    va_list ap;
    va_start(ap, count);

    int total = 0;
    for (int i = 0; i < count; i++)
        total += va_arg(ap, int);

    va_end(ap);
    return total;
}

/* Maksimum değer — vprintf tarzı: format string ile tip belirleme */
static double max_of(int count, ...)
{
    va_list ap;
    va_start(ap, count);

    double max = va_arg(ap, double);
    for (int i = 1; i < count; i++)
    {
        double v = va_arg(ap, double);
        if (v > max) max = v;
    }

    va_end(ap);
    return max;
}

/* va_copy: aynı argüman listesini iki kez kullanma */
static void print_args_twice(int count, ...)
{
    va_list ap, ap_copy;
    va_start(ap, count);
    va_copy(ap_copy, ap);   /* ap'nin yedeği */

    printf("pass1: ");
    for (int i = 0; i < count; i++)
        printf("%d ", va_arg(ap, int));
    printf("\n");

    printf("pass2: ");
    for (int i = 0; i < count; i++)
        printf("%d ", va_arg(ap_copy, int));
    printf("\n");

    va_end(ap_copy);
    va_end(ap);
}

static void demo_variadic(void)
{
    printf("sum(1,2,3)     = %d\n", sum_variadic(3, 1, 2, 3));
    printf("sum(1..5)      = %d\n", sum_variadic(5, 1, 2, 3, 4, 5));
    printf("max(1.5,9.2,3) = %.1f\n", max_of(3, 1.5, 9.2, 3.0));
    print_args_twice(4, 10, 20, 30, 40);
}

/* ===========================================================================
 * BÖLÜM 4 — Recursive fonksiyon
 *
 * Her özyinelemeli çağrı stack frame kullanır.
 * Çok derin özyineleme → stack overflow.
 *
 * Kuyruk özyinelemesi (tail recursion):
 *   Özyinelemeli çağrı fonksiyonun son işlemi — bazı derleyiciler
 *   bunu iteratif koda dönüştürür (TCO: Tail Call Optimization).
 *   GCC/Clang -O2 ile TCO uygular.
 *
 * Genel kural: n > ~10.000 için iteratif tercih edilmeli.
 * ===========================================================================*/

/* Klasik özyineleme — O(n) stack */
static int factorial(int n)
{
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

/* Kuyruk özyinelemesi — TCO uygulanabilir */
static int factorial_tail(int n, int acc)
{
    if (n <= 1) return acc;
    return factorial_tail(n - 1, n * acc);   /* son işlem: özyinelemeli çağrı */
}

/* Binary search — özyinelemeli, O(log n) stack */
static int binary_search(const int *arr, int left, int right, int target)
{
    if (left > right) return -1;

    int mid = left + (right - left) / 2;   /* taşma önlemi */

    if (arr[mid] == target) return mid;
    if (arr[mid] < target)  return binary_search(arr, mid + 1, right, target);
    return                         binary_search(arr, left, mid - 1, target);
}

static void demo_recursive(void)
{
    printf("factorial(5)         = %d\n", factorial(5));
    printf("factorial_tail(5,1)  = %d\n", factorial_tail(5, 1));

    int arr[] = {1, 3, 5, 7, 9, 11, 13};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));

    printf("binary_search(7) at index %d\n",
           binary_search(arr, 0, n - 1, 7));
    printf("binary_search(4) = %d (not found)\n",
           binary_search(arr, 0, n - 1, 4));
}

/* ===========================================================================
 * BÖLÜM 5 — Callback
 *
 * Fonksiyon pointer parametre olarak geçilir.
 * Çağıran kod hangi işlemin yapılacağını belirlemiş olur.
 *
 * Yaygın kullanımlar: qsort, event handler, middleware pattern.
 * ===========================================================================*/
typedef void (*Processor_t)(int *, int);

static void double_elements(int *arr, int n)
{
    for (int i = 0; i < n; i++) arr[i] *= 2;
}

static void negate_elements(int *arr, int n)
{
    for (int i = 0; i < n; i++) arr[i] = -arr[i];
}

/* apply: callback alan generic fonksiyon */
static void apply(int *arr, int n, Processor_t fn)
{
    if (fn) fn(arr, n);
}

static void demo_callback(void)
{
    int arr[] = {1, 2, 3, 4, 5};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));

    apply(arr, n, double_elements);
    printf("doubled: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    apply(arr, n, negate_elements);
    printf("negated: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    /* NULL callback güvenli geçilebilir — apply içinde kontrol var */
    apply(arr, n, NULL);
}

/* ===========================================================================
 * BÖLÜM 6 — Array parametre
 *
 * Fonksiyona dizi geçildiğinde pointer'a decay olur.
 * Boyut bilgisi kaybolur — ayrıca geçirilmeli.
 *
 * Yöntemler:
 *   1. Boyutu ayrı parametre geçir (en yaygın)
 *   2. Sentinel değer kullan (NULL, -1 vb.)
 *   3. Struct içinde sarmalama
 * ===========================================================================*/
typedef struct { int *data; int size; } IntArray_t;

/* Yöntem 1: boyutu ayrı geçir */
static int array_sum(const int *arr, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return sum;
}

/* Yöntem 2: sentinel (-1 ile biter) */
static int array_sum_sentinel(const int *arr)
{
    int sum = 0;
    while (*arr != -1) sum += *arr++;
    return sum;
}

/* Yöntem 3: struct içinde sarmalama */
static int array_sum_struct(IntArray_t a)
{
    int sum = 0;
    for (int i = 0; i < a.size; i++) sum += a.data[i];
    return sum;
}

static void demo_array_param(void)
{
    int arr[]      = {1, 2, 3, 4, 5};
    int sentinel[] = {1, 2, 3, 4, 5, -1};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));

    printf("sum (size param)   = %d\n", array_sum(arr, n));
    printf("sum (sentinel)     = %d\n", array_sum_sentinel(sentinel));
    printf("sum (struct)       = %d\n", array_sum_struct((IntArray_t){arr, n}));
}

/* ===========================================================================
 * BÖLÜM 7 — Struct döndürme ve error handling
 *
 * Return struct (C99+): küçük struct'lar register üzerinden döner (RVO).
 * Compound literal: geçici struct oluşturma.
 *
 * Error handling pattern:
 *   1. Return code: 0 başarı, negatif hata
 *   2. errno: POSIX hata kodu
 *   3. Out-parameter: sonucu pointer ile döndür, return kodu hata için
 * ===========================================================================*/
typedef struct
{
    int   code;
    char  message[64];
} Result_t;

/* Struct döndürme — compound literal ile */
static Result_t make_result(int code, const char *msg)
{
    Result_t r = { .code = code };
    strncpy(r.message, msg, sizeof(r.message) - 1);
    return r;   /* RVO: derleyici kopyayı optimize edebilir */
}

/* Out-parameter pattern: sonuç pointer ile, hata return code ile */
static int safe_divide(int a, int b, int *out)
{
    if (b == 0)
    {
        errno = EDOM;   /* errno: domain error */
        return -1;
    }
    *out = a / b;
    return 0;
}

static void demo_return_and_error(void)
{
    /* Struct döndürme */
    Result_t ok  = make_result(0, "success");
    Result_t err = make_result(-1, "not found");

    printf("ok:  code=%d msg=%s\n", ok.code,  ok.message);
    printf("err: code=%d msg=%s\n", err.code, err.message);

    /* errno pattern */
    int result;
    if (safe_divide(10, 2, &result) == 0)
        printf("10 / 2 = %d\n", result);

    if (safe_divide(10, 0, &result) != 0)
    {
        /* perror: errno mesajını stderr'e yazar */
        perror("safe_divide");
        printf("errno = %d\n", errno);
    }
}

/* ===========================================================================
 * main
 * ===========================================================================*/
int main(void)
{
    printf(LINE);
    LOG_INFO("functions.c — starting demos");
    printf(LINE);

    demo_parameter_passing(); printf(LINE);
    demo_inline();            printf(LINE);
    demo_variadic();          printf(LINE);
    demo_recursive();         printf(LINE);
    demo_callback();          printf(LINE);
    demo_array_param();       printf(LINE);
    demo_return_and_error();  printf(LINE);

    LOG_INFO("functions.c — all demos complete");
    return 0;
}