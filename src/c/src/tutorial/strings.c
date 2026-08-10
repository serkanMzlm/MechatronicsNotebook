/*
 * strings.c — String ve Bellek İşlemleri
 *
 * Kapsanan konular:
 *   char array vs pointer → stack kopya vs read-only literal
 *   null terminator       → boyut hesabı, güvenli kullanım
 *   strlen/strcpy/strcat  → güvenli ve güvensiz versiyonlar
 *   strcmp/strncmp        → sıralama, eşitlik kontrolü
 *   strchr/strrchr/strstr → karakter ve substring arama
 *   strtok/strtok_r       → token parçalama, thread-safety
 *   strtol/strtod         → güvenli string → sayı dönüşümü
 *   memcpy/memmove/memset → ham bellek işlemleri
 *   memchr/memcmp         → ham bellek arama ve karşılaştırma
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>    /* toupper, tolower, isdigit, isalpha */
#include <errno.h>

#include "common.h"

/* ===========================================================================
 * BÖLÜM 1 — char array vs string pointer
 *
 * char arr[] = "hello"  → stack'te null-terminated kopya, değiştirilebilir
 * char *ptr  = "hello"  → read-only segment, değiştirilemez (UB)
 *
 * sizeof farkı:
 *   sizeof(arr) → 6 (null dahil)
 *   sizeof(ptr) → 8 (pointer boyutu, 64-bit)
 *
 * strlen farkı:
 *   strlen(arr) → 5 (null hariç)
 * ===========================================================================*/
static void demo_char_array_vs_ptr(void)
{
    char        arr[] = "hello";   /* stack kopya — değiştirilebilir */
    const char *ptr   = "hello";   /* read-only — const ile koruma */

    printf("arr = %s | sizeof=%zu | strlen=%zu\n",
           arr, sizeof(arr), strlen(arr));
    printf("ptr = %s | sizeof=%zu | strlen=%zu\n",
           ptr, sizeof(ptr), strlen(ptr));

    /* arr değiştirilebilir */
    arr[0] = 'H';
    printf("arr after modify: %s\n", arr);

    /* ptr[0] = 'H'; → undefined behavior — const ile önlenir */

    /* Null terminator gösterimi */
    char buf[] = {'A', 'B', 'C', '\0'};   /* elle null */
    printf("manual null-term: %s (len=%zu)\n", buf, strlen(buf));
}

/* ===========================================================================
 * BÖLÜM 2 — strlen, strcpy, strncpy, strcat, strncat
 *
 * Güvensiz versiyonlar (buffer overflow riski):
 *   strcpy  → hedef yeterliyse sorun yok, değilse taşar
 *   strcat  → aynı risk
 *   gets    → ASLA kullanma (standarttan kaldırıldı)
 *
 * Güvenli versiyonlar:
 *   strncpy(dst, src, n) → n karakter kopyalar, null garanti etmez (!)
 *   strncat(dst, src, n) → n karakter ekler, null garantiler
 *
 * En güvenli yol: snprintf veya strlcpy (BSD, POSIX olmayan)
 * ===========================================================================*/
static void demo_copy_concat(void)
{
    char dst[16];
    const char *src = "Hello";

    /* strcpy — yeterli alan varsa güvenli */
    strcpy(dst, src);
    printf("strcpy: %s\n", dst);

    /* strncpy — n karakter kopyalar, null garanti etmez */
    char safe[8] = {0};                     /* sıfırla başlat */
    strncpy(safe, "TooLong!!", sizeof(safe) - 1);
    /* safe[sizeof(safe)-1] zaten 0 — sıfırlama sayesinde güvenli */
    printf("strncpy: %s\n", safe);

    /* strcat — hedefte yeterli alan olmalı */
    char result[32] = "Hello";
    strcat(result, ", World");
    printf("strcat: %s\n", result);

    /* strncat — n karakter ekler, null garantiler */
    strncat(result, "!!!", sizeof(result) - strlen(result) - 1);
    printf("strncat: %s\n", result);

    /* snprintf — en güvenli string birleştirme */
    char final[32];
    snprintf(final, sizeof(final), "%s %s", "Hello", "CMake");
    printf("snprintf concat: %s\n", final);
}

/* ===========================================================================
 * BÖLÜM 3 — strcmp, strncmp
 *
 * strcmp return değeri:
 *   < 0 → s1 < s2 (alfabetik sıralamada s1 önce)
 *   = 0 → eşit
 *   > 0 → s1 > s2
 *
 * strncmp: sadece ilk n karakteri karşılaştırır
 * strcasecmp: büyük/küçük harf duyarsız (POSIX, taşınabilir değil)
 * ===========================================================================*/
static void demo_compare(void)
{
    const char *s1 = "apple";
    const char *s2 = "banana";
    const char *s3 = "apple";

    printf("strcmp(\"%s\",\"%s\") = %d\n", s1, s2, strcmp(s1, s2));   /* < 0 */
    printf("strcmp(\"%s\",\"%s\") = %d\n", s2, s1, strcmp(s2, s1));   /* > 0 */
    printf("strcmp(\"%s\",\"%s\") = %d\n", s1, s3, strcmp(s1, s3));   /* = 0 */

    /* strncmp: prefix karşılaştırma */
    printf("strncmp(\"abcdef\",\"abcxyz\",3) = %d\n",
           strncmp("abcdef", "abcxyz", 3));   /* 0 — ilk 3 eşit */

    /* String sıralama — qsort + strcmp callback */
    const char *words[] = {"cherry", "apple", "banana", "apricot"};
    int n = (int)(sizeof(words) / sizeof(words[0]));

    /* strcmp doğrudan kullanılamaz (void* cast gerekli) */
    int cmp_str(const void *a, const void *b)
    {
        return strcmp(*(const char **)a, *(const char **)b);
    }
    qsort(words, n, sizeof(char *), cmp_str);

    printf("sorted: ");
    for (int i = 0; i < n; i++) printf("%s ", words[i]);
    printf("\n");
}

/* ===========================================================================
 * BÖLÜM 4 — strchr, strrchr, strstr
 *
 * strchr(s, c)    → ilk c karakterinin pointer'ı (NULL: bulunamadı)
 * strrchr(s, c)   → son c karakterinin pointer'ı
 * strstr(hay, ndl)→ needle'ın haystack'teki ilk konumu
 *
 * Pointer aritmetiği ile konum hesabı: result - str = index
 * ===========================================================================*/
static void demo_search(void)
{
    const char *text = "hello world hello";

    /* strchr — ilk 'o' */
    char *first = strchr(text, 'o');
    if (first)
        printf("first 'o' at index %td: ...%s\n", first - text, first);

    /* strrchr — son 'o' */
    char *last = strrchr(text, 'o');
    if (last)
        printf("last  'o' at index %td: ...%s\n", last - text, last);

    /* strstr — substring arama */
    char *found = strstr(text, "world");
    if (found)
        printf("'world' at index %td\n", found - text);

    /* Tüm eşleşmeleri bul */
    const char *needle = "hello";
    const char *pos = text;
    printf("all '%s' positions: ", needle);
    while ((pos = strstr(pos, needle)) != NULL)
    {
        printf("%td ", pos - text);
        pos += strlen(needle);
    }
    printf("\n");
}

/* ===========================================================================
 * BÖLÜM 5 — strtok ve strtok_r
 *
 * strtok: delimiter'a göre string'i parçalar.
 *   - Orijinal string'i MODIFIYE eder (delimiter yerine \0 koyar)
 *   - İlk çağrı: strtok(str, delim)
 *   - Sonraki çağrılar: strtok(NULL, delim)
 *   - Thread-safe DEĞİL (iç static state kullanır)
 *
 * strtok_r (POSIX): thread-safe versiyon, saveptr ile durum dışarıda tutulur.
 * ===========================================================================*/
static void demo_strtok(void)
{
    /* strtok orijinali değiştirdiği için kopya üzerinde çalış */
    char csv[]  = "alice,30,engineer,ankara";
    char *token;
    char *rest  = csv;
    int   field = 0;
    const char *labels[] = {"name", "age", "title", "city"};

    printf("CSV parse:\n");

    /* strtok_r — thread-safe, rest saveptr */
    while ((token = strtok_r(rest, ",", &rest)) != NULL)
    {
        printf("  %s = %s\n", labels[field++], token);
    }

    /* Çoklu delimiter */
    char path[] = "/usr/local/bin/gcc";
    char *part  = strtok(path, "/");
    printf("path parts: ");
    while (part != NULL)
    {
        printf("[%s] ", part);
        part = strtok(NULL, "/");
    }
    printf("\n");
}

/* ===========================================================================
 * BÖLÜM 6 — strtol, strtod (güvenli string → sayı dönüşümü)
 *
 * atoi/atof: hata durumunda belirsiz davranış (0 döner, hata tespiti yok)
 * strtol/strtod: hatalı karakterin pointer'ını döner, errno set eder
 *
 * strtol(str, &endptr, base):
 *   endptr → dönüşümün durduğu noktayı işaret eder
 *   base   → 0: otomatik (0x=hex, 0=octal, diğer=decimal)
 * ===========================================================================*/
static void demo_string_to_number(void)
{
    const char *inputs[] = {"42", "  -17", "3.14", "0xFF", "123abc", "abc"};
    int n = (int)(sizeof(inputs) / sizeof(inputs[0]));

    for (int i = 0; i < n; i++)
    {
        char   *endptr;
        errno = 0;
        long val = strtol(inputs[i], &endptr, 0);

        if (endptr == inputs[i])
            printf("'%s' → no conversion\n", inputs[i]);
        else if (*endptr != '\0')
            printf("'%s' → %ld (trailing: '%s')\n", inputs[i], val, endptr);
        else
            printf("'%s' → %ld (clean)\n", inputs[i], val);
    }

    /* strtod — ondalık sayı */
    char  *end;
    double d = strtod("3.14159xyz", &end);
    printf("strtod: %.5f (stopped at '%s')\n", d, end);
}

/* ===========================================================================
 * BÖLÜM 7 — memcpy, memmove, memset, memchr, memcmp
 *
 * memcpy(dst, src, n)  → n byte kopyalar, ÖRTÜŞME OLMAMALI
 * memmove(dst, src, n) → n byte kopyalar, örtüşme güvenli
 * memset(ptr, val, n)  → n byte'ı val ile doldurur
 * memchr(ptr, val, n)  → ilk val byte'ının pointer'ı
 * memcmp(a, b, n)      → n byte karşılaştırır (strcmp gibi dönüş)
 * ===========================================================================*/
static void demo_memory_ops(void)
{
    /* memset — başlatma */
    uint8_t buf[8];
    memset(buf, 0xAB, sizeof(buf));
    printf("memset 0xAB: ");
    for (int i = 0; i < 8; i++) printf("%02X ", buf[i]);
    printf("\n");

    memset(buf, 0, sizeof(buf));   /* sıfırlama */

    /* memcpy — örtüşme yok */
    uint8_t src[] = {1, 2, 3, 4, 5};
    uint8_t dst[5];
    memcpy(dst, src, sizeof(src));
    printf("memcpy: ");
    for (int i = 0; i < 5; i++) printf("%d ", dst[i]);
    printf("\n");

    /* memmove — örtüşen bölge güvenli kopyalama */
    uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8};
    memmove(data + 2, data, 5);   /* 2 byte sağa kaydır */
    printf("memmove shift: ");
    for (int i = 0; i < 8; i++) printf("%d ", data[i]);
    printf("\n");

    /* memchr — byte arama */
    uint8_t haystack[] = {0x10, 0x20, 0x30, 0x40, 0x50};
    uint8_t *found = memchr(haystack, 0x30, sizeof(haystack));
    if (found)
        printf("memchr 0x30 at index %td\n", found - haystack);

    /* memcmp — binary karşılaştırma */
    uint8_t a[] = {1, 2, 3};
    uint8_t b[] = {1, 2, 4};
    printf("memcmp: %d\n", memcmp(a, b, sizeof(a)));   /* < 0 */
}

/* ===========================================================================
 * BÖLÜM 8 — ctype.h karakter sınıflandırma ve dönüşüm
 *
 * isalpha, isdigit, isalnum, isspace, isupper, islower
 * toupper, tolower
 *
 * Dikkat: argüman unsigned char veya EOF olmalı.
 *   char c ile çağırmak UB olabilir (negatif char değerleri)
 *   Güvenli: (unsigned char)c
 * ===========================================================================*/
static void demo_ctype(void)
{
    const char *sample = "Hello, World! 123";

    printf("original : %s\n", sample);
    printf("uppercase: ");
    for (const char *p = sample; *p; p++)
        putchar(toupper((unsigned char)*p));
    printf("\n");

    /* Sadece alfanumerik karakterleri filtrele */
    printf("alnum only: ");
    for (const char *p = sample; *p; p++)
        if (isalnum((unsigned char)*p))
            putchar(*p);
    printf("\n");

    /* Baştaki/sondaki boşlukları trim et */
    char trimmed[] = "   trim me   ";
    char *start = trimmed;
    while (isspace((unsigned char)*start)) start++;

    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    printf("trimmed: [%s]\n", start);
}

/* ===========================================================================
 * main
 * ===========================================================================*/
int main(void)
{
    printf(LINE);
    LOG_INFO("strings.c — starting demos");
    printf(LINE);

    demo_char_array_vs_ptr(); printf(LINE);
    demo_copy_concat();       printf(LINE);
    demo_compare();           printf(LINE);
    demo_search();            printf(LINE);
    demo_strtok();            printf(LINE);
    demo_string_to_number();  printf(LINE);
    demo_memory_ops();        printf(LINE);
    demo_ctype();             printf(LINE);

    LOG_INFO("strings.c — all demos complete");
    return 0;
}