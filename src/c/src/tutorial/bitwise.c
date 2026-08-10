/*
 * bitwise.c — Bit Operasyonları
 *
 * Kapsanan konular:
 *   Temel operatörler → &, |, ^, ~, <<, >>
 *   Maskeleme         → bit set, clear, toggle, check
 *   Flag register     → bitmask pattern, donanım simülasyonu
 *   Bit tricks        → swap, power of 2, LSB/MSB, popcount
 *   Signed shift      → aritmetik vs mantıksal shift farkı
 *   Endianness        → byte sırası ve swap
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"

/* Bit değerini binary string olarak yazdırır */
static void print_binary(uint32_t val, int bits)
{
    for (int i = bits - 1; i >= 0; i--)
        putchar((val >> i) & 1 ? '1' : '0');
}

/* ===========================================================================
 * BÖLÜM 1 — Temel bitwise operatörler
 *
 * &  → AND:  her iki bit 1 ise 1
 * |  → OR:   en az bir bit 1 ise 1
 * ^  → XOR:  bitler farklıysa 1 (aynıysa 0)
 * ~  → NOT:  bitleri tersine çevirir (bitwise complement)
 * << → left shift:  n bit sola kaydır (× 2^n)
 * >> → right shift: n bit sağa kaydır (÷ 2^n)
 *
 * Dikkat:
 *   ~ işaretsiz: ~0u → tüm bitler 1
 *   ~ işaretli:  ~0  → -1 (two's complement)
 *   Negatif sayı sağa shift: aritmetik mi mantıksal mı? → tanımsız (C'de)
 * ===========================================================================*/
static void demo_basic_operators(void)
{
    uint8_t a = 0b10110100;   /* 180 */
    uint8_t b = 0b01101110;   /* 110 */

    printf("a     = "); print_binary(a, 8); printf(" (%u)\n", a);
    printf("b     = "); print_binary(b, 8); printf(" (%u)\n", b);
    printf("a & b = "); print_binary(a & b, 8); printf(" (%u)\n", a & b);
    printf("a | b = "); print_binary(a | b, 8); printf(" (%u)\n", a | b);
    printf("a ^ b = "); print_binary(a ^ b, 8); printf(" (%u)\n", a ^ b);
    printf("~a    = "); print_binary((uint8_t)~a, 8); printf(" (%u)\n", (uint8_t)~a);
    printf("a<<1  = "); print_binary((uint8_t)(a << 1), 8); printf(" (%u)\n", (uint8_t)(a << 1));
    printf("a>>1  = "); print_binary(a >> 1, 8); printf(" (%u)\n", a >> 1);
}

/* ===========================================================================
 * BÖLÜM 2 — Bit maskeleme: set, clear, toggle, check
 *
 * SET    : reg |=  (1u << bit)    → biti 1 yap
 * CLEAR  : reg &= ~(1u << bit)    → biti 0 yap
 * TOGGLE : reg ^=  (1u << bit)    → biti tersine çevir
 * CHECK  : reg &   (1u << bit)    → bit 1 mi?
 *
 * common.h'da static inline versiyonları tanımlı:
 *   bit_set(), bit_clear(), bit_is_set()
 * ===========================================================================*/
static void demo_masking(void)
{
    uint8_t reg = 0x00;

    /* SET */
    reg |= (1u << 3);   /* bit 3'ü set et */
    reg |= (1u << 6);   /* bit 6'yı set et */
    printf("after set  3,6 : "); print_binary(reg, 8); printf("\n");

    /* CLEAR */
    reg &= ~(1u << 3);  /* bit 3'ü temizle */
    printf("after clear 3  : "); print_binary(reg, 8); printf("\n");

    /* TOGGLE */
    reg ^= (1u << 6);   /* bit 6'yı toggle */
    reg ^= (1u << 7);   /* bit 7'yi toggle */
    printf("after toggle 6,7: "); print_binary(reg, 8); printf("\n");

    /* CHECK */
    for (int i = 7; i >= 0; i--)
    {
        if (reg & (1u << i))
            printf("bit %d is set\n", i);
    }

    /* Çoklu bit maskesi */
    uint8_t mask  = 0b00111100;   /* bit 2-5 */
    uint8_t val   = 0b10110110;
    uint8_t field = (val & mask) >> 2;   /* bit 2-5'i ayıkla, sağa kaydır */
    printf("extracted field [5:2] = %u\n", field);
}

/* ===========================================================================
 * BÖLÜM 3 — Donanım register simülasyonu
 *
 * Gerçek donanımda memory-mapped I/O register'ları bu şekilde yönetilir.
 * volatile eklenmeliydi (donanım için) — burada simülasyon amaçlı.
 * ===========================================================================*/
typedef enum
{
    CTRL_ENABLE    = (1u << 0),   /* bit 0: enable */
    CTRL_DIRECTION = (1u << 1),   /* bit 1: yön    */
    CTRL_IRQ_EN    = (1u << 2),   /* bit 2: kesme  */
    CTRL_RESET     = (1u << 7),   /* bit 7: reset  */
} CtrlBits_t;

static void demo_register_sim(void)
{
    uint8_t CTRL_REG = 0x00;

    /* Birden fazla biti aynı anda set et */
    CTRL_REG |= (CTRL_ENABLE | CTRL_IRQ_EN);
    printf("CTRL after init  : "); print_binary(CTRL_REG, 8); printf("\n");

    /* Reset bitini geçici set et, ardından temizle */
    CTRL_REG |= CTRL_RESET;
    printf("CTRL with reset  : "); print_binary(CTRL_REG, 8); printf("\n");
    CTRL_REG &= ~CTRL_RESET;
    printf("CTRL after reset : "); print_binary(CTRL_REG, 8); printf("\n");

    /* Durum sorgula */
    printf("ENABLE  : %s\n", (CTRL_REG & CTRL_ENABLE)    ? "on" : "off");
    printf("IRQ_EN  : %s\n", (CTRL_REG & CTRL_IRQ_EN)    ? "on" : "off");
    printf("DIRECTION: %s\n", (CTRL_REG & CTRL_DIRECTION) ? "on" : "off");
}

/* ===========================================================================
 * BÖLÜM 4 — Bit tricks
 *
 * Yaygın bit manipülasyon teknikleri.
 * Gömülü sistemlerde ve performans kritik kodda sık kullanılır.
 * ===========================================================================*/
static void demo_bit_tricks(void)
{
    uint32_t x;

    /* XOR swap — geçici değişken olmadan iki değeri takas et
     * NOT: a == b ise yanlış sonuç verir (0 olur) — kullanımda dikkat */
    uint32_t a = 0xABCD, b = 0x1234;
    a ^= b;
    b ^= a;
    a ^= b;
    printf("XOR swap: a=0x%04X b=0x%04X\n", a, b);

    /* 2'nin kuvveti kontrolü: n > 0 && (n & (n-1)) == 0 */
    for (x = 0; x <= 20; x++)
    {
        if (x > 0 && (x & (x - 1)) == 0)
            printf("%u is power of 2\n", x);
    }

    /* LSB (En düşük set bit) bulma: x & (-x) */
    x = 0b10110100;
    uint32_t lsb = x & (uint32_t)(-(int32_t)x);
    printf("LSB of %u : "); print_binary(lsb, 8); printf("\n");

    /* En düşük set biti temizle: x & (x-1) */
    printf("clear LSB: "); print_binary(x & (x - 1), 8); printf("\n");

    /* Popcount: set bit sayısı (Manuel — __builtin_popcount ile de yapılabilir) */
    x = 0b10110101;
    uint32_t count = 0;
    uint32_t tmp = x;
    while (tmp)
    {
        count += tmp & 1;
        tmp >>= 1;
    }
    printf("popcount(%u) = %u\n", x, count);

    /* Derleyici built-in — genellikle tek CPU talimatına iner */
    printf("__builtin_popcount(%u) = %d\n", x, __builtin_popcount(x));

    /* Byte'ları ters çevirme — 32-bit */
    x = 0x12345678;
    uint32_t rev = ((x & 0xFF000000) >> 24) |
                   ((x & 0x00FF0000) >>  8) |
                   ((x & 0x0000FF00) <<  8) |
                   ((x & 0x000000FF) << 24);
    printf("byte_reverse(0x%08X) = 0x%08X\n", x, rev);
}

/* ===========================================================================
 * BÖLÜM 5 — Signed shift davranışı
 *
 * Left shift (<<):
 *   unsigned → well-defined (n bit sola, sağdan sıfır girer)
 *   signed   → taşma durumu undefined behavior (C'de)
 *
 * Right shift (>>):
 *   unsigned → mantıksal shift (soldan sıfır girer)
 *   signed   → aritmetik shift (soldan işaret biti girer) — implementation-defined
 *              GCC/Clang pratikte aritmetik shift uygular
 *
 * Güvenli kural: shift operasyonlarında unsigned kullan.
 * ===========================================================================*/
static void demo_shift(void)
{
    /* unsigned shift — well-defined */
    uint8_t u = 0b10000001;
    printf("u      = "); print_binary(u, 8); printf("\n");
    printf("u << 1 = "); print_binary((uint8_t)(u << 1), 8);
    printf(" (sol taşma: MSB kaybolur)\n");
    printf("u >> 1 = "); print_binary(u >> 1, 8);
    printf(" (sağ: soldan 0 girer)\n");

    /* signed shift — implementation-defined sağa shift */
    int8_t s = (int8_t)0b10000001;   /* -127 */
    printf("s      = %d\n", s);
    printf("s >> 1 = %d (aritmetik: işaret biti korunur)\n", s >> 1);

    /* Çarpma/bölme kısayolu — sadece unsigned veya pozitif signed ile güvenli */
    uint32_t val = 7;
    printf("%u << 3 = %u (× 8)\n", val, val << 3);
    printf("%u >> 2 = %u (÷ 4)\n", val, val >> 2);

    /* Bit alanı çıkarma ve yerleştirme */
    uint32_t reg   = 0x00000000;
    uint32_t field = 0b1010;     /* 4-bitlik değer */
    int      pos   = 4;          /* başlangıç bit pozisyonu */
    uint32_t fmask = 0xF;        /* 4-bit mask */

    /* Yerleştir */
    reg |= (field & fmask) << pos;
    printf("after insert [7:4]=0b1010: 0x%08X\n", reg);

    /* Çıkar */
    uint32_t extracted = (reg >> pos) & fmask;
    printf("extracted [7:4]: 0b"); print_binary(extracted, 4); printf("\n");
}

/* ===========================================================================
 * BÖLÜM 6 — Endianness
 *
 * Little-endian: LSB düşük adreste (x86, ARM LE)
 * Big-endian:    MSB düşük adreste (ağ protokolleri, bazı ARM)
 *
 * htons/htonl: host → network byte order (big-endian)
 * ntohs/ntohl: network → host byte order
 *
 * Burada manuel implementasyon gösterilir.
 * ===========================================================================*/
static void demo_endianness(void)
{
    uint32_t val = 0x12345678;
    uint8_t *bytes = (uint8_t *)&val;

    printf("0x%08X byte order: ", val);
    for (int i = 0; i < 4; i++)
        printf("[%d]=0x%02X ", i, bytes[i]);
    printf("\n");

    if (bytes[0] == 0x78)
        printf("System: little-endian\n");
    else
        printf("System: big-endian\n");

    /* Manuel byte swap (htonl eşdeğeri) */
    uint32_t swapped = ((val & 0xFF000000) >> 24) |
                       ((val & 0x00FF0000) >>  8) |
                       ((val & 0x0000FF00) <<  8) |
                       ((val & 0x000000FF) << 24);
    printf("byte_swapped: 0x%08X\n", swapped);

    /* __builtin_bswap32 — derleyici built-in, genellikle tek talimat */
    printf("bswap32: 0x%08X\n", __builtin_bswap32(val));
    printf("bswap16: 0x%04X\n", __builtin_bswap16((uint16_t)val));
}

/* ===========================================================================
 * main
 * ===========================================================================*/
int main(void)
{
    printf(LINE);
    LOG_INFO("bitwise.c — starting demos");
    printf(LINE);

    demo_basic_operators(); printf(LINE);
    demo_masking();         printf(LINE);
    demo_register_sim();    printf(LINE);
    demo_bit_tricks();      printf(LINE);
    demo_shift();           printf(LINE);
    demo_endianness();      printf(LINE);

    LOG_INFO("bitwise.c — all demos complete");
    return 0;
}