/*
 * time_demo.c — Zaman İşlemleri
 *
 * Kapsanan konular:
 *   time()      → epoch saniyesi
 *   localtime() → yerel zaman yapısı
 *   gmtime()    → UTC zaman yapısı
 *   strftime()  → zaman formatlama
 *   difftime()  → iki zaman arası fark
 *   clock()     → CPU kullanım süresi
 *   timespec    → nanosaniye hassasiyeti (C11)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "common.h"

/* ===========================================================================
 * BÖLÜM 1 — time_t ve struct tm
 *
 * time_t       → epoch'tan (1970-01-01 00:00:00 UTC) geçen saniye
 * struct tm    → insan okunabilir zaman bileşenleri
 *   tm_year    → 1900'den itibaren yıl
 *   tm_mon     → 0-11 (Ocak=0)
 *   tm_mday    → 1-31
 *   tm_hour    → 0-23
 *   tm_min     → 0-59
 *   tm_sec     → 0-60 (artık saniye)
 *   tm_wday    → 0-6 (Pazar=0)
 *   tm_yday    → 0-365
 *   tm_isdst   → yaz saati uygulaması
 * ===========================================================================*/
static void demo_time_types(void)
{
    printf("--- time_t / struct tm ---\n");

    time_t now = time(NULL);   /* mevcut epoch saniyesi */
    printf("epoch     : %ld\n", (long)now);

    /* localtime: yerel saat dilimine göre */
    struct tm *local = localtime(&now);
    printf("local     : %04d-%02d-%02d %02d:%02d:%02d\n",
           local->tm_year + 1900,
           local->tm_mon  + 1,
           local->tm_mday,
           local->tm_hour,
           local->tm_min,
           local->tm_sec);

    /* gmtime: UTC */
    struct tm *utc = gmtime(&now);
    printf("UTC       : %04d-%02d-%02d %02d:%02d:%02d\n",
           utc->tm_year + 1900,
           utc->tm_mon  + 1,
           utc->tm_mday,
           utc->tm_hour,
           utc->tm_min,
           utc->tm_sec);

    printf("weekday   : %d (0=Sun)\n", local->tm_wday);
    printf("day of yr : %d\n",         local->tm_yday);
}

/* ===========================================================================
 * BÖLÜM 2 — strftime
 *
 * printf benzeri zaman formatlama.
 *
 * Yaygın format specifier'lar:
 *   %Y → 4 haneli yıl      %y → 2 haneli yıl
 *   %m → ay (01-12)        %d → gün (01-31)
 *   %H → saat (00-23)      %M → dakika   %S → saniye
 *   %A → tam gün adı       %a → kısa gün adı
 *   %B → tam ay adı        %b → kısa ay adı
 *   %c → yerel tarih+saat  %X → yerel saat
 *   %Z → zaman dilimi      %s → epoch saniyesi (GNU)
 * ===========================================================================*/
static void demo_strftime(void)
{
    printf("--- strftime ---\n");

    time_t    now = time(NULL);
    struct tm *t  = localtime(&now);
    char       buf[64];

    strftime(buf, sizeof(buf), "%Y-%m-%d", t);
    printf("date     : %s\n", buf);

    strftime(buf, sizeof(buf), "%H:%M:%S", t);
    printf("time     : %s\n", buf);

    strftime(buf, sizeof(buf), "%A, %B %d %Y", t);
    printf("long     : %s\n", buf);

    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", t);
    printf("custom   : %s\n", buf);

    strftime(buf, sizeof(buf), "%c", t);
    printf("locale   : %s\n", buf);
}

/* ===========================================================================
 * BÖLÜM 3 — difftime ve mktime
 *
 * difftime(t2, t1) → t2 - t1 farkını double olarak döner (saniye)
 * mktime(tm)       → struct tm'yi time_t'ye çevirir
 * ===========================================================================*/
static void demo_difftime(void)
{
    printf("--- difftime / mktime ---\n");

    time_t now = time(NULL);

    /* Gelecek bir tarihi mktime ile time_t'ye çevir */
    struct tm future = *localtime(&now);
    future.tm_mday += 7;     /* 7 gün sonra */
    future.tm_hour  = 0;
    future.tm_min   = 0;
    future.tm_sec   = 0;
    time_t next_week = mktime(&future);

    double diff = difftime(next_week, now);
    printf("seconds to next week : %.0f\n", diff);
    printf("hours   to next week : %.1f\n", diff / 3600.0);
    printf("days    to next week : %.1f\n", diff / 86400.0);

    /* İki fixed tarihi karşılaştır */
    struct tm t1 = { .tm_year=124, .tm_mon=0, .tm_mday=1 };  /* 2024-01-01 */
    struct tm t2 = { .tm_year=124, .tm_mon=11, .tm_mday=31}; /* 2024-12-31 */
    double year_secs = difftime(mktime(&t2), mktime(&t1));
    printf("2024 duration        : %.0f days\n", year_secs / 86400.0);
}

/* ===========================================================================
 * BÖLÜM 4 — clock() ile CPU süresi ölçümü
 *
 * clock()          → işlemci tik sayısı (CLOCKS_PER_SEC ile saniyeye çevir)
 * clock_t          → tik sayısı tipi
 * CLOCKS_PER_SEC   → saniyedeki tik sayısı (genellikle 1000000)
 *
 * Dikkat: wall-clock değil CPU süresi ölçer.
 *         Uyku (sleep) süresini saymaz.
 * ===========================================================================*/
static void demo_clock(void)
{
    printf("--- clock() ---\n");

    clock_t start = clock();

    /* CPU yoğun işlem */
    volatile double sum = 0.0;
    for (int i = 0; i < 10000000; i++)
        sum += sqrt((double)i);

    clock_t end     = clock();
    double  elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("sum      : %.2f\n", sum);
    printf("CPU time : %.6f seconds\n", elapsed);
    printf("CLOCKS_PER_SEC: %ld\n", (long)CLOCKS_PER_SEC);
}

/* ===========================================================================
 * BÖLÜM 5 — timespec (C11 / POSIX)
 *
 * Nanosaniye hassasiyetinde zaman.
 * timespec_get(): C11 standart fonksiyon.
 *
 * struct timespec:
 *   tv_sec  → saniye
 *   tv_nsec → nanosaniye (0-999999999)
 * ===========================================================================*/
static void demo_timespec(void)
{
    printf("--- timespec ---\n");

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);

    printf("tv_sec  : %ld\n", ts.tv_sec);
    printf("tv_nsec : %ld\n", ts.tv_nsec);

    /* Yüksek hassasiyetli ölçüm */
    struct timespec t1, t2;
    timespec_get(&t1, TIME_UTC);

    volatile long x = 0;
    for (long i = 0; i < 1000000L; i++) x += i;

    timespec_get(&t2, TIME_UTC);

    long ns = (t2.tv_sec - t1.tv_sec) * 1000000000L
            + (t2.tv_nsec - t1.tv_nsec);

    printf("loop result : %ld\n", x);
    printf("elapsed     : %ld ns (%.3f ms)\n", ns, ns / 1e6);
}

/* ===========================================================================
 * main
 * ===========================================================================*/
int main(void)
{
    printf(LINE);
    LOG_INFO("time_demo.c");
    printf(LINE);

    demo_time_types(); printf(LINE);
    demo_strftime();   printf(LINE);
    demo_difftime();   printf(LINE);
    demo_clock();      printf(LINE);
    demo_timespec();   printf(LINE);

    return 0;
}