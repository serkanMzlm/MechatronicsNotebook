/*
 * pointers.c — Pointer Kavramları
 *
 * Kapsanan konular:
 *   Temel pointer      → adres, dereference, NULL kontrolü
 *   Pointer aritmetiği → ++/--, fark, sınır kontrolü
 *   void pointer       → generic bellek işlemleri
 *   Function pointer   → tanım, dizi, callback pattern
 *   Double pointer     → **ptr, out-parameter, dinamik 2D dizi
 *   const pointer      → 3 kombinasyon detaylı
 *   Pointer vs array   → decay, sizeof farkı
 *   restrict           → aliasing optimizasyonu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

/* ===========================================================================
 * BÖLÜM 1 — Temel pointer
 *
 * Pointer: bir değişkenin bellek adresini tutan değişken.
 *
 * &  → adres alma operatörü
 * *  → dereference (adresteki değere erişim)
 *
 * NULL pointer dereference → undefined behavior, genellikle segfault.
 * Her pointer kullanımından önce NULL kontrolü yapılmalı.
 * ===========================================================================*/
static void demo_basic_pointer(void)
{
    int   value = 42;
    int  *ptr   = &value;   /* ptr, value'nun adresini tutar */

    printf("value    = %d\n",    value);
    printf("&value   = %p\n",    (void *)&value);
    printf("ptr      = %p\n",    (void *)ptr);
    printf("*ptr     = %d\n",    *ptr);

    /* Dereference ile değer değiştirme */
    *ptr = 100;
    printf("*ptr=100 → value = %d\n", value);

    /* NULL kontrolü */
    int *null_ptr = NULL;
    if (null_ptr == NULL)
        LOG_INFO("null_ptr is NULL — safe to skip dereference");
}

/* ===========================================================================
 * BÖLÜM 2 — Pointer aritmetiği
 *
 * Pointer +/- integer: sizeof(type) * n byte ilerleme.
 * Pointer - pointer: aradaki eleman sayısı (ptrdiff_t).
 *
 * Geçerli pointer aritmetiği:
 *   - Aynı dizi içinde veya dizinin bir ötesi (past-the-end)
 * Geçersiz:
 *   - Dizi dışı → undefined behavior
 *   - Farklı nesnelerin pointer'ları arasında çıkarma
 * ===========================================================================*/
static void demo_pointer_arithmetic(void)
{
    int arr[] = {10, 20, 30, 40, 50};
    int *ptr  = arr;         /* dizi adı pointer'a decay olur */
    int  n    = (int)(sizeof(arr) / sizeof(arr[0]));

    printf("--- pointer arithmetic ---\n");

    /* İlerleme */
    for (int i = 0; i < n; i++)
        printf("ptr+%d → %p = %d\n", i, (void *)(ptr + i), *(ptr + i));

    /* ++ operatörü */
    ptr = arr;
    printf("*ptr++: %d, %d\n", *ptr++, *ptr);   /* önce oku, sonra ilerle */

    /* Pointer farkı */
    int *start = arr;
    int *end   = arr + n;
    printf("end - start = %td elements\n", end - start);  /* ptrdiff_t → %td */

    /* Sınır kontrolü — güvenli iterasyon */
    ptr = arr;
    while (ptr < arr + n)
    {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
}

/* ===========================================================================
 * BÖLÜM 3 — void pointer
 *
 * void*: tip bilgisi olmayan generic pointer.
 *   - malloc/free void* döner ve alır
 *   - memcpy, memset gibi stdlib fonksiyonları void* kullanır
 *   - Kullanmadan önce doğru tipe cast edilmeli
 *   - void* dereference edilemez (boyut bilinmiyor)
 *   - C'de void* ile diğer pointer tipleri arasında implicit cast var
 *     (C++'da explicit cast gerekir)
 * ===========================================================================*/
static void demo_void_pointer(void)
{
    int    i = 42;
    float  f = 3.14f;
    char   s[] = "hello";

    void *ptr;

    /* Farklı tipleri aynı void* ile tutabiliriz */
    ptr = &i;
    printf("void*→int:   %d\n",  *(int *)ptr);

    ptr = &f;
    printf("void*→float: %.2f\n", *(float *)ptr);

    ptr = s;
    printf("void*→char*: %s\n",  (char *)ptr);

    /* Generic swap — void* ile tip bağımsız */
    void generic_swap(void *a, void *b, size_t size);
    char x = 'A', y = 'Z';
    generic_swap(&x, &y, sizeof(char));
    printf("after swap: x=%c y=%c\n", x, y);
}

/* Generic swap implementasyonu — memcpy ile byte bazlı takas */
void generic_swap(void *a, void *b, size_t size)
{
    /* VLA yerine sabit boyutlu geçici buffer */
    unsigned char tmp[256];
    if (size > sizeof(tmp)) return;

    memcpy(tmp, a,   size);
    memcpy(a,   b,   size);
    memcpy(b,   tmp, size);
}

/* ===========================================================================
 * BÖLÜM 4 — Function pointer
 *
 * Sözdizimi: return_type (*name)(param_types)
 *
 * Kullanım yerleri:
 *   - Callback (qsort, sinyal handler)
 *   - Strateji pattern (runtime'da algoritma seçimi)
 *   - Dispatch table (switch yerine)
 * ===========================================================================*/

/* Callback için fonksiyon tipleri */
typedef int (*CompareFn_t)(const void *, const void *);
typedef void (*ActionFn_t)(int);

static int compare_asc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

static int compare_desc(const void *a, const void *b)
{
    return (*(int *)b - *(int *)a);
}

static void print_value(int v)  { printf("%d ", v); }
static void square_value(int v) { printf("%d ", v * v); }

static void demo_function_pointer(void)
{
    /* Temel kullanım */
    int (*add_ptr)(int, int);
    add_ptr = NULL;

    /* Lambda-like inline atama */
    int result;
    {
        int local_add(int a, int b) { return a + b; }   /* GCC nested func */
        add_ptr = local_add;
        result  = add_ptr(3, 5);
    }
    printf("add via ptr: %d\n", result);

    /* Dispatch table: switch yerine function pointer dizisi */
    ActionFn_t actions[] = { print_value, square_value };
    const char *labels[] = { "print", "square" };

    int values[] = {2, 3, 4};
    for (int a = 0; a < 2; a++)
    {
        printf("%s: ", labels[a]);
        for (int i = 0; i < 3; i++)
            actions[a](values[i]);
        printf("\n");
    }

    /* qsort callback — stdlib sort, kullanıcı karşılaştırma fonksiyonu */
    int arr[] = {5, 2, 8, 1, 9, 3};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));

    qsort(arr, n, sizeof(int), compare_asc);
    printf("asc:  "); for (int i=0;i<n;i++) printf("%d ",arr[i]); printf("\n");

    qsort(arr, n, sizeof(int), compare_desc);
    printf("desc: "); for (int i=0;i<n;i++) printf("%d ",arr[i]); printf("\n");

    /* typedef ile temiz sözdizimi */
    CompareFn_t cmp = compare_asc;
    printf("compare(1,2)=%d\n", cmp(&(int){1}, &(int){2}));
}

/* ===========================================================================
 * BÖLÜM 5 — Double pointer (**)
 *
 * Kullanım yerleri:
 *   1. Out-parameter: fonksiyon içinde pointer'ı değiştirmek
 *   2. Dinamik 2D dizi
 *   3. Pointer dizileri (string dizisi vb.)
 * ===========================================================================*/

/* Out-parameter: fonksiyon içinde pointer'ı değiştirir */
static void allocate_buffer(char **out, size_t size)
{
    *out = malloc(size);   /* *out → çağıranın pointer'ını değiştirir */
    if (*out)
        memset(*out, 0, size);
}

static void demo_double_pointer(void)
{
    /* Out-parameter kullanımı */
    char *buf = NULL;
    allocate_buffer(&buf, 64);
    if (buf)
    {
        snprintf(buf, 64, "allocated via double pointer");
        printf("%s\n", buf);
        free(buf);
    }

    /* Dinamik 2D dizi */
    int rows = 3, cols = 4;
    int **matrix = malloc(rows * sizeof(int *));

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
            matrix[i][j] = i * cols + j;
    }

    printf("2D matrix:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            printf("%3d", matrix[i][j]);
        printf("\n");
    }

    /* Serbest bırakma sırası: önce satırlar, sonra dizi */
    for (int i = 0; i < rows; i++)
        free(matrix[i]);
    free(matrix);
}

/* ===========================================================================
 * BÖLÜM 6 — Pointer vs Array
 *
 * Array decay: dizi ismi, dizinin ilk elemanının pointer'ına dönüşür.
 *   - sizeof farkı: sizeof(arr) dizi boyutu, sizeof(ptr) pointer boyutu
 *   - Fonksiyona geçildiğinde dizi pointer'a decay olur — boyut kaybolur
 *
 * String literal:
 *   char *s  = "hello"  → read-only memory'de, s[0]='H' → UB
 *   char s[] = "hello"  → stack'te kopya, değiştirilebilir
 * ===========================================================================*/
static void demo_pointer_vs_array(void)
{
    int  arr[] = {1, 2, 3, 4, 5};
    int *ptr   = arr;         /* decay: arr → &arr[0] */

    printf("sizeof(arr) = %zu\n",  sizeof(arr));    /* 20 (5 * 4) */
    printf("sizeof(ptr) = %zu\n",  sizeof(ptr));    /* 8 (64-bit) */
    printf("arr == ptr  : %d\n",   arr == ptr);     /* 1 (aynı adres) */

    /* Array indexing ve pointer aritmetiği eşdeğer */
    printf("arr[2] = %d | *(ptr+2) = %d\n", arr[2], *(ptr + 2));

    /* String literal farkı */
    const char *str_ptr = "immutable";   /* read-only — değiştirme */
    char        str_arr[] = "mutable";   /* stack kopya — değiştirilebilir */

    str_arr[0] = 'M';
    printf("str_ptr: %s\n", str_ptr);
    printf("str_arr: %s\n", str_arr);
}

/* ===========================================================================
 * BÖLÜM 7 — restrict
 *
 * restrict (C99): derleyiciye "bu pointer'ın gösterdiği belleğe
 * başka pointer erişmiyor" garantisi verir.
 *
 * Aliasing: iki pointer'ın aynı belleği göstermesi.
 * restrict olmadan derleyici aliasing varsayar → daha az optimizasyon.
 * restrict ile derleyici yeniden yükleme yapmadan optimize edebilir.
 *
 * Yanlış kullanım (aliasing varken restrict) → undefined behavior.
 * ===========================================================================*/

/* restrict olmadan: derleyici her adımda bellekten yeniden okuyabilir */
static void add_arrays(int *dst, const int *src, int n)
{
    for (int i = 0; i < n; i++)
        dst[i] += src[i];
}

/* restrict ile: derleyici dst ve src'nin örtüşmediğini bilir → optimize eder */
static void add_arrays_restrict(int * restrict dst,
                                const int * restrict src, int n)
{
    for (int i = 0; i < n; i++)
        dst[i] += src[i];
}

static void demo_restrict(void)
{
    int a[] = {1, 2, 3, 4, 5};
    int b[] = {10, 20, 30, 40, 50};

    add_arrays(a, b, 5);
    printf("add_arrays:          ");
    for (int i = 0; i < 5; i++) printf("%d ", a[i]);
    printf("\n");

    int c[] = {1, 2, 3, 4, 5};
    int d[] = {10, 20, 30, 40, 50};

    add_arrays_restrict(c, d, 5);
    printf("add_arrays_restrict: ");
    for (int i = 0; i < 5; i++) printf("%d ", c[i]);
    printf("\n");
}

/* ===========================================================================
 * main
 * ===========================================================================*/
int main(void)
{
    printf(LINE);
    LOG_INFO("pointers.c — starting demos");
    printf(LINE);

    demo_basic_pointer();     printf(LINE);
    demo_pointer_arithmetic(); printf(LINE);
    demo_void_pointer();      printf(LINE);
    demo_function_pointer();  printf(LINE);
    demo_double_pointer();    printf(LINE);
    demo_pointer_vs_array();  printf(LINE);
    demo_restrict();          printf(LINE);

    LOG_INFO("pointers.c — all demos complete");
    return 0;
}