/*
 * structs.c — Struct, Union, Enum ve Bileşik Tipler
 *
 * Kapsanan konular:
 *   struct          → tanım, başlatma, üye erişim, kopyalama
 *   typedef struct  → anonim ve isimli struct farkı
 *   struct pointer  → ->, (*ptr). erişim
 *   nested struct   → iç içe struct
 *   flexible array  → struct sonunda esnek dizi (C99)
 *   bitfield        → donanım register, __attribute__((packed))
 *   union           → memory overlay, type punning
 *   enum            → bitmask flag pattern
 *   designated init → C99 .field = value
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "common.h"

/* ===========================================================================
 * BÖLÜM 1 — Temel struct
 *
 * struct: farklı tipteki verileri bir arada tutar.
 *
 * typedef struct farkı:
 *   struct Point { int x; int y; };   → struct Point p; (struct anahtar kelimesi gerekli)
 *   typedef struct { int x; int y; } Point_t;  → Point_t p; (daha kısa)
 *
 * İsimli typedef:
 *   typedef struct Node { struct Node *next; } Node_t;
 *   → Self-referencing (linked list) için isim zorunlu.
 * ===========================================================================*/

/* Anonim struct — typedef olmadan kullanılamaz */
typedef struct
{
    float x;
    float y;
} Point_t;

/* İsimli struct — self-referencing için zorunlu */
typedef struct Node
{
    int          value;
    struct Node *next;   /* kendi tipine pointer — isim olmadan yazılamaz */
} Node_t;

static void demo_basic_struct(void)
{
    /* Sıralı başlatma */
    Point_t p1 = {1.0f, 2.0f};

    /* Designated initializer (C99) — sıra bağımsız, okunabilir */
    Point_t p2 = { .x = 5.0f, .y = 3.0f };

    /* Belirtilmeyen alanlar sıfırlanır */
    Point_t p3 = { .x = 7.0f };   /* y = 0.0 */

    printf("p1=(%.1f, %.1f)\n", p1.x, p1.y);
    printf("p2=(%.1f, %.1f)\n", p2.x, p2.y);
    printf("p3=(%.1f, %.1f)\n", p3.x, p3.y);

    /* Struct kopyalama: değer kopyası (shallow copy) */
    Point_t p4 = p1;
    p4.x = 99.0f;
    printf("p1.x=%.1f (unchanged), p4.x=%.1f\n", p1.x, p4.x);

    /* sizeof struct: padding eklenebilir */
    printf("sizeof(Point_t) = %zu\n", sizeof(Point_t));
}

/* ===========================================================================
 * BÖLÜM 2 — Struct pointer erişimi
 *
 * ptr->field   == (*ptr).field
 * İkisi eşdeğer; -> daha yaygın ve okunabilir.
 * ===========================================================================*/
typedef struct
{
    char    name[32];
    int     age;
    float   score;
} Student_t;

static void print_student(const Student_t *s)
{
    /* const pointer: s değiştirilemez, içerik de değiştirilemez */
    printf("name=%-12s age=%d score=%.1f\n", s->name, s->age, s->score);
}

static void demo_struct_pointer(void)
{
    Student_t s = { .name = "Alice", .age = 20, .score = 92.5f };

    /* Stack struct — pointer ile erişim */
    Student_t *ptr = &s;
    printf("via ->  : name=%s\n", ptr->name);
    printf("via (*) : name=%s\n", (*ptr).name);

    /* Fonksiyona pointer geçirme — kopyalamadan erişim */
    print_student(&s);

    /* Heap struct */
    Student_t *heap_s = malloc(sizeof(Student_t));
    if (!heap_s) return;

    strncpy(heap_s->name, "Bob", sizeof(heap_s->name) - 1);
    heap_s->age   = 22;
    heap_s->score = 88.0f;
    print_student(heap_s);
    free(heap_s);
}

/* ===========================================================================
 * BÖLÜM 3 — Nested struct
 *
 * Struct içinde struct — composit veri modeli.
 * Erişim: obj.inner.field veya ptr->inner.field
 *
 * Dikkat: nested struct kopyalanırken derin kopyalama olmaz (shallow copy).
 * Pointer üyeler varsa aynı belleği paylaşırlar.
 * ===========================================================================*/
typedef struct
{
    int day;
    int month;
    int year;
} Date_t;

typedef struct
{
    char   title[64];
    float  salary;
    Date_t start_date;   /* nested struct — değer olarak gömülü */
} Employee_t;

static void demo_nested_struct(void)
{
    Employee_t emp = {
        .title      = "Engineer",
        .salary     = 75000.0f,
        .start_date = { .day = 1, .month = 9, .year = 2022 }
    };

    printf("title  : %s\n",    emp.title);
    printf("salary : %.0f\n",  emp.salary);
    printf("start  : %02d/%02d/%04d\n",
           emp.start_date.day,
           emp.start_date.month,
           emp.start_date.year);

    /* Pointer ile nested erişim */
    Employee_t *eptr = &emp;
    printf("via ptr: %02d/%02d/%04d\n",
           eptr->start_date.day,
           eptr->start_date.month,
           eptr->start_date.year);
}

/* ===========================================================================
 * BÖLÜM 4 — Flexible array member (C99)
 *
 * Struct'ın son üyesi boyutsuz dizi olabilir.
 * Dinamik boyutlu veri yapıları için kullanılır (mesaj, paket vb.)
 *
 * Kısıtlar:
 *   - Sadece son üye olabilir
 *   - sizeof'a dahil edilmez
 *   - En az bir başka üye olmalı
 * ===========================================================================*/
typedef struct
{
    uint32_t id;
    uint16_t length;
    uint8_t  data[];   /* flexible array — boyut dinamik belirlenir */
} Packet_t;

static void demo_flexible_array(void)
{
    uint16_t payload_size = 8;

    /* sizeof(Packet_t) data[] dahil değil — manuel hesap */
    Packet_t *pkt = malloc(sizeof(Packet_t) + payload_size);
    if (!pkt) return;

    pkt->id     = 0xDEAD;
    pkt->length = payload_size;

    for (int i = 0; i < payload_size; i++)
        pkt->data[i] = (uint8_t)(i * 10);

    printf("packet id=0x%04X len=%u data=",
           pkt->id, pkt->length);
    for (int i = 0; i < payload_size; i++)
        printf("%02X ", pkt->data[i]);
    printf("\n");

    free(pkt);
}

/* ===========================================================================
 * BÖLÜM 5 — Bitfield
 *
 * Struct üyelerinin bit seviyesinde boyutunu belirtir.
 *
 * Kullanım yerleri:
 *   - Donanım register haritası
 *   - Protokol header'ları
 *   - Bellek kısıtlı gömülü sistemler
 *
 * Dikkat:
 *   - Bit sırası (MSB/LSB) platforma bağlı
 *   - __attribute__((packed)) padding'i kaldırır
 *   - Taşınabilirlik için explicit mask/shift tercih edilebilir
 * ===========================================================================*/
typedef struct __attribute__((packed))
{
    uint8_t enable   : 1;   /* bit 0 */
    uint8_t mode     : 2;   /* bit 1-2 */
    uint8_t channel  : 4;   /* bit 3-6 */
    uint8_t reserved : 1;   /* bit 7 */
} Register_t;

_Static_assert(sizeof(Register_t) == 1, "Register_t must be 1 byte");

static void demo_bitfield(void)
{
    Register_t reg = {0};
    reg.enable  = 1;
    reg.mode    = 2;     /* 0b10 */
    reg.channel = 7;     /* 0b0111 */

    printf("enable=%u mode=%u channel=%u\n",
           reg.enable, reg.mode, reg.channel);
    printf("sizeof(Register_t)=%zu byte(s)\n", sizeof(Register_t));

    /* Ham byte değeri — packed sayesinde 1 byte */
    uint8_t raw;
    memcpy(&raw, &reg, 1);
    printf("raw byte: 0x%02X\n", raw);
}

/* ===========================================================================
 * BÖLÜM 6 — Union
 *
 * Union: tüm üyeler aynı bellek bölgesini paylaşır.
 * Boyutu en büyük üyenin boyutuna eşittir.
 *
 * Kullanım yerleri:
 *   1. Memory overlay — aynı veriye farklı görünüm
 *   2. Type punning — float bit pattern'ini okuma
 *   3. Tagged union — tip güvenli variant
 * ===========================================================================*/

/* Memory overlay: 32-bit değere byte, word, dword erişimi */
typedef union
{
    uint32_t dword;
    uint16_t word[2];
    uint8_t  byte[4];
} Register32_t;

/* Type punning — float IEEE 754 bit pattern */
typedef union
{
    float    f;
    uint32_t bits;
} FloatUnion_t;

/* Tagged union — hangi üyenin geçerli olduğunu enum ile işaret eder */
typedef enum { TYPE_INT, TYPE_FLOAT, TYPE_STRING } ValueType_t;

typedef struct
{
    ValueType_t type;
    union
    {
        int         i;
        float       f;
        const char *s;
    } as;
} Value_t;

static void demo_union(void)
{
    /* Memory overlay */
    Register32_t reg = { .dword = 0xDEADBEEF };
    printf("dword : 0x%08X\n", reg.dword);
    printf("word[0]: 0x%04X word[1]: 0x%04X\n", reg.word[0], reg.word[1]);
    printf("bytes : %02X %02X %02X %02X\n",
           reg.byte[0], reg.byte[1], reg.byte[2], reg.byte[3]);

    /* Type punning */
    FloatUnion_t fu = { .f = 1.0f };
    printf("1.0f bits: 0x%08X\n", fu.bits);   /* 0x3F800000 */

    fu.f = -0.0f;
    printf("-0.0f bits: 0x%08X\n", fu.bits);   /* 0x80000000 */

    /* Tagged union */
    Value_t vals[] = {
        { .type = TYPE_INT,    .as.i = 42       },
        { .type = TYPE_FLOAT,  .as.f = 3.14f    },
        { .type = TYPE_STRING, .as.s = "hello"  },
    };

    for (int i = 0; i < 3; i++)
    {
        switch (vals[i].type)
        {
            case TYPE_INT:    printf("int:    %d\n",   vals[i].as.i); break;
            case TYPE_FLOAT:  printf("float:  %.2f\n", vals[i].as.f); break;
            case TYPE_STRING: printf("string: %s\n",   vals[i].as.s); break;
        }
    }
}

/* ===========================================================================
 * BÖLÜM 7 — Enum ve bitmask flag pattern
 *
 * Bitmask enum: her değer bir biti temsil eder.
 * Birden fazla flag aynı anda aktif olabilir (|= ile set, &~ ile clear).
 * ===========================================================================*/
typedef enum
{
    PERM_NONE    = 0,
    PERM_READ    = (1 << 0),   /* 0b001 */
    PERM_WRITE   = (1 << 1),   /* 0b010 */
    PERM_EXECUTE = (1 << 2),   /* 0b100 */
    PERM_ALL     = PERM_READ | PERM_WRITE | PERM_EXECUTE
} Permission_t;

static void demo_enum_bitmask(void)
{
    Permission_t perms = PERM_NONE;

    /* Flag set */
    perms |= PERM_READ;
    perms |= PERM_WRITE;
    printf("perms after set: 0x%X\n", perms);

    /* Flag kontrol */
    if (perms & PERM_READ)    printf("READ ok\n");
    if (perms & PERM_WRITE)   printf("WRITE ok\n");
    if (!(perms & PERM_EXECUTE)) printf("EXECUTE denied\n");

    /* Flag clear */
    perms &= ~PERM_WRITE;
    printf("perms after clear WRITE: 0x%X\n", perms);

    /* Toggle */
    perms ^= PERM_EXECUTE;
    printf("perms after toggle EXECUTE: 0x%X\n", perms);
}

/* ===========================================================================
 * main
 * ===========================================================================*/
int main(void)
{
    printf(LINE);
    LOG_INFO("structs.c — starting demos");
    printf(LINE);

    demo_basic_struct();    printf(LINE);
    demo_struct_pointer();  printf(LINE);
    demo_nested_struct();   printf(LINE);
    demo_flexible_array();  printf(LINE);
    demo_bitfield();        printf(LINE);
    demo_union();           printf(LINE);
    demo_enum_bitmask();    printf(LINE);

    LOG_INFO("structs.c — all demos complete");
    return 0;
}