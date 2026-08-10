/*
 * io.c — Giriş/Çıkış İşlemleri
 *
 * Kapsanan konular:
 *   printf        → format specifier'lar, padding, precision
 *   scanf         → return value kontrolü, buffer temizleme
 *   sprintf/snprintf → buffer'a yazma, güvenli versiyon
 *   sscanf        → string'den parse
 *   getchar/putchar → karakter bazlı I/O
 *   fgets/fputs   → satır bazlı I/O
 *   fopen/fclose  → dosya açma modları
 *   fread/fwrite  → binary I/O
 *   fprintf/fscanf → dosyaya/dosyadan formatlı I/O
 *   fseek/ftell/rewind → dosya konumu
 *   feof/ferror   → hata kontrolü
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#define TEST_FILE  "io_test.txt"
#define BIN_FILE   "io_test.bin"

/* ===========================================================================
 * BÖLÜM 1 — printf format specifier'lar
 *
 * Genel format: %[flags][width][.precision][length]type
 *
 * Flags:
 *   -   → sola yasla
 *   +   → işareti her zaman göster
 *   0   → sıfır ile doldur
 *   #   → alternatif format (0x prefix, vb.)
 *   ' ' → pozitif sayıda boşluk bırak
 *
 * Length modifier:
 *   h  → short      (hh → char)
 *   l  → long       (ll → long long)
 *   z  → size_t
 *   L  → long double
 * ===========================================================================*/
static void demo_printf(void)
{
    int   i   = 42;
    float f   = 3.14159f;
    char *s   = "hello";
    void *ptr = &i;

    printf("--- printf format specifiers ---\n");

    /* Tam sayı */
    printf("[%d]   decimal\n",      i);
    printf("[%5d]  width=5 right\n", i);
    printf("[%-5d] width=5 left\n",  i);
    printf("[%05d] zero-padded\n",   i);
    printf("[%+d]  show sign\n",     i);

    /* Taban dönüşümleri */
    printf("[%o]   octal\n",     i);
    printf("[%x]   hex lower\n", i);
    printf("[%X]   hex upper\n", i);
    printf("[%#x]  hex prefix\n", i);

    /* Ondalık */
    printf("[%f]      default float\n",    f);
    printf("[%.2f]    precision=2\n",      f);
    printf("[%8.3f]   width=8 prec=3\n",  f);
    printf("[%-8.3f]  left-aligned\n",     f);
    printf("[%e]      scientific\n",       f);
    printf("[%g]      shorter of f/e\n",   f);

    /* String ve char */
    printf("[%s]    string\n",      s);
    printf("[%10s]  width=10\n",    s);
    printf("[%.3s]  max 3 chars\n", s);
    printf("[%c]    char\n",        'A');

    /* Pointer ve boyut */
    printf("[%p]    pointer\n",     ptr);
    printf("[%zu]   size_t\n",      sizeof(int));

    /* Dinamik genişlik: * ile argümandan alınır */
    printf("[%*.*f] dynamic w=8 p=3\n", 8, 3, f);
}

/* ===========================================================================
 * BÖLÜM 2 — scanf ve buffer temizleme
 *
 * scanf return değeri: başarıyla okunan eleman sayısı.
 *   0   → hiç okuyamadı
 *   EOF → stream kapandı veya hata
 *
 * Yaygın hatalar:
 *   - Return değeri kontrol edilmez
 *   - Giriş buffer'ı temizlenmez → sonraki scanf'i etkiler
 *   - %s için boyut belirtilmez → buffer overflow
 *
 * Buffer temizleme:
 *   scanf("%*[^\n]%*c") → satır sonuna kadar atla, newline'ı tüket
 * ===========================================================================*/
static void demo_scanf(void)
{
    int   num;
    float val;
    char  name[32];

    printf("Enter an integer: ");

    /* Return value kontrolü zorunlu */
    if (scanf("%d", &num) != 1)
    {
        LOG_ERROR("Invalid integer input");
        /* Buffer'ı temizle: newline ve geçersiz karakterleri at */
        scanf("%*[^\n]%*c");
        return;
    }

    /* Buffer temizleme: bir sonraki okuma için hazırla */
    scanf("%*[^\n]%*c");

    printf("Enter a float: ");
    if (scanf("%f", &val) != 1)
    {
        LOG_ERROR("Invalid float input");
        scanf("%*[^\n]%*c");
        return;
    }
    scanf("%*[^\n]%*c");

    printf("Enter name (max 31 chars): ");
    /* %31s: en fazla 31 karakter okur, buffer overflow önler */
    if (scanf("%31s", name) != 1)
    {
        LOG_ERROR("Invalid name input");
        return;
    }

    printf("Read: int=%d float=%.2f name=%s\n", num, val, name);
}

/* ===========================================================================
 * BÖLÜM 3 — sprintf ve snprintf
 *
 * sprintf:  buffer boyutu kontrolü yapmaz → overflow riski
 * snprintf: maksimum n-1 karakter yazar, null terminator garantiler
 *           return değeri: yazılmak istenen karakter sayısı (kesilmiş olabilir)
 *
 * Kesilme kontrolü: return >= n ise çıktı kesikdir.
 * ===========================================================================*/
static void demo_sprintf(void)
{
    char   buf[64];
    char   small[10];
    int    n;

    /* sprintf — boyut kontrolü yok, küçük örnek için kabul edilebilir */
    sprintf(buf, "DeviceID=%d Temp=%.2f", 42, 36.5f);
    printf("sprintf: %s\n", buf);

    /* snprintf — güvenli versiyon */
    n = snprintf(buf, sizeof(buf), "Version=%d.%d.%d", 1, 0, 0);
    printf("snprintf: %s (wrote %d chars)\n", buf, n);

    /* Kesilme tespiti */
    n = snprintf(small, sizeof(small), "LongString=%d", 123456);
    if (n >= (int)sizeof(small))
        LOG_WARN("snprintf truncated! needed %d, had %zu", n, sizeof(small));

    printf("truncated: [%s]\n", small);
}

/* ===========================================================================
 * BÖLÜM 4 — sscanf
 *
 * String içinden formatlı veri parse eder.
 * scanf ile aynı format specifier'ları kullanır.
 * Network protokolü, config dosyası parse için yaygın kullanılır.
 * ===========================================================================*/
static void demo_sscanf(void)
{
    const char *data = "sensor=12 value=98.6 unit=C";

    int   sensor_id;
    float value;
    char  unit[4];

    int matched = sscanf(data, "sensor=%d value=%f unit=%3s",
                         &sensor_id, &value, unit);

    printf("sscanf matched %d fields\n", matched);
    if (matched == 3)
        printf("sensor=%d value=%.1f unit=%s\n", sensor_id, value, unit);
}

/* ===========================================================================
 * BÖLÜM 5 — getchar / putchar
 *
 * getchar: stdin'den tek byte okur, int döner (EOF = -1 kontrol için)
 * putchar: stdout'a tek byte yazar
 *
 * getchar int döner çünkü EOF (-1) char ile temsil edilemez.
 * ===========================================================================*/
static void demo_getchar(void)
{
    int ch;

    printf("Type something (Enter to stop): ");
    fflush(stdout);

    /* getchar döngüsü: newline veya EOF'a kadar oku */
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* Büyük harfe çevirerek geri yaz */
        putchar((ch >= 'a' && ch <= 'z') ? ch - 32 : ch);
    }
    putchar('\n');
}

/* ===========================================================================
 * BÖLÜM 6 — fgets / fputs
 *
 * fgets: satır bazlı güvenli okuma — n-1 karakter veya newline'a kadar okur
 *   - '\n' karakterini buffer'a dahil eder
 *   - gets() ASLA kullanılmamalı (buffer overflow, standarttan kaldırıldı)
 *
 * fputs: string yazar, '\n' eklemez (puts ekler)
 * ===========================================================================*/
static void demo_fgets(void)
{
    char line[128];

    printf("Enter a line: ");
    fflush(stdout);

    if (fgets(line, sizeof(line), stdin) == NULL)
    {
        LOG_ERROR("fgets failed");
        return;
    }

    /* fgets '\n' dahil eder — temizle */
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';

    fputs("You entered: ", stdout);
    fputs(line, stdout);
    putchar('\n');
}

/* ===========================================================================
 * BÖLÜM 7 — fopen / fclose ve text dosya I/O
 *
 * Açma modları:
 *   "r"  → okuma (dosya yoksa NULL)
 *   "w"  → yazma (dosyayı sıfırlar veya oluşturur)
 *   "a"  → ekleme (dosya sonuna yazar)
 *   "r+" → okuma+yazma (dosya mevcut olmalı)
 *   "w+" → okuma+yazma (dosyayı sıfırlar)
 *   "b"  → binary mod (rb, wb, ab ...)
 *
 * fopen NULL dönebilir — her zaman kontrol et.
 * fclose dönüş değeri: flush hatalarını yakalar.
 * ===========================================================================*/
static void demo_text_file(void)
{
    /* Yazma */
    FILE *fp = fopen(TEST_FILE, "w");
    if (!fp)
    {
        /* perror: errno'yu insan okunabilir hata mesajına çevirir */
        perror("fopen write");
        return;
    }

    fprintf(fp, "line1: %d\n", 100);
    fprintf(fp, "line2: %.2f\n", 3.14f);
    fprintf(fp, "line3: %s\n", "hello");

    if (fclose(fp) != 0)
        LOG_WARN("fclose may have failed (flush error?)");

    /* Okuma */
    fp = fopen(TEST_FILE, "r");
    if (!fp) { perror("fopen read"); return; }

    char line[64];
    printf("--- text file content ---\n");

    /* fgets ile satır satır okuma */
    while (fgets(line, sizeof(line), fp) != NULL)
        fputs(line, stdout);

    /* feof / ferror: döngü neden bitti? */
    if (feof(fp))
        printf("(reached end of file)\n");
    if (ferror(fp))
        LOG_ERROR("file read error");

    fclose(fp);
}

/* ===========================================================================
 * BÖLÜM 8 — fread / fwrite (binary I/O)
 *
 * fwrite(ptr, size, count, fp) → count adet size-byte blok yazar
 *   return: yazılan blok sayısı (count'tan azsa hata var)
 *
 * fread(ptr, size, count, fp) → count adet size-byte blok okur
 *   return: okunan blok sayısı
 *
 * Binary I/O dikkat:
 *   - Endianness: farklı mimaride okunamayabilir
 *   - Struct padding: derleyiciye göre değişir
 *   - Taşınabilirlik için XDR/protobuf gibi format tercih edilir
 * ===========================================================================*/
typedef struct
{
    uint32_t id;
    float    temperature;
    uint8_t  status;
} SensorRecord_t;

static void demo_binary_file(void)
{
    SensorRecord_t records[] = {
        {1, 36.5f, 1},
        {2, 37.1f, 1},
        {3, 35.8f, 0},
    };
    size_t count = sizeof(records) / sizeof(records[0]);

    /* Binary yazma */
    FILE *fp = fopen(BIN_FILE, "wb");
    if (!fp) { perror("fopen binary write"); return; }

    /* Header: kayıt sayısını yaz */
    fwrite(&count, sizeof(count), 1, fp);

    /* Kayıtları yaz */
    size_t written = fwrite(records, sizeof(SensorRecord_t), count, fp);
    if (written != count)
        LOG_WARN("fwrite: wrote %zu of %zu records", written, count);

    fclose(fp);

    /* Binary okuma */
    fp = fopen(BIN_FILE, "rb");
    if (!fp) { perror("fopen binary read"); return; }

    size_t read_count;
    fread(&read_count, sizeof(read_count), 1, fp);

    SensorRecord_t *buf = malloc(read_count * sizeof(SensorRecord_t));
    if (!buf) { fclose(fp); return; }

    fread(buf, sizeof(SensorRecord_t), read_count, fp);
    fclose(fp);

    printf("--- binary file records ---\n");
    for (size_t i = 0; i < read_count; i++)
    {
        printf("id=%u temp=%.1f status=%u\n",
               buf[i].id, buf[i].temperature, buf[i].status);
    }

    free(buf);
}

/* ===========================================================================
 * BÖLÜM 9 — fseek / ftell / rewind
 *
 * fseek(fp, offset, whence):
 *   SEEK_SET → dosya başından
 *   SEEK_CUR → mevcut konumdan
 *   SEEK_END → dosya sonundan
 *
 * ftell: mevcut konumu byte cinsinden döner
 * rewind: dosya başına döner, hata flag'ini temizler
 * ===========================================================================*/
static void demo_fseek(void)
{
    FILE *fp = fopen(TEST_FILE, "r");
    if (!fp) { perror("fopen seek"); return; }

    /* Dosya boyutunu ölç */
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    printf("File size: %ld bytes\n", size);

    /* Dosya başına dön */
    rewind(fp);

    /* İlk 5 karakteri oku */
    char peek[6] = {0};
    fread(peek, 1, 5, fp);
    printf("First 5 chars: [%s]\n", peek);

    /* 10. byte'a atla */
    fseek(fp, 10, SEEK_SET);
    printf("Position after seek: %ld\n", ftell(fp));

    fclose(fp);

    /* Temizlik */
    remove(TEST_FILE);
    remove(BIN_FILE);
}

/* ===========================================================================
 * main
 * ===========================================================================*/
int main(void)
{
    printf(LINE);
    LOG_INFO("io.c — starting demos");
    printf(LINE);

    demo_printf();    printf(LINE);
    demo_sprintf();   printf(LINE);
    demo_sscanf();    printf(LINE);
    demo_text_file(); printf(LINE);
    demo_binary_file(); printf(LINE);
    demo_fseek();     printf(LINE);

    /*
     * Aşağıdaki demolar kullanıcı girdisi gerektirir.
     * Otomatik test ortamında yorum satırına alınabilir.
     */
    /* demo_scanf();   */
    /* demo_getchar(); */
    /* demo_fgets();   */

    LOG_INFO("io.c — all demos complete");
    return 0;
}