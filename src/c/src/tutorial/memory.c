/*
 * memory.c — Dinamik Bellek Yönetimi
 *
 * Kapsanan konular:
 *   malloc/calloc/realloc/free → temel kullanım ve farklar
 *   safe wrapper               → NULL kontrolü, hata yönetimi
 *   dangling pointer           → free sonrası tehlike ve önlem
 *   double free                → nasıl oluşur, nasıl önlenir
 *   memory leak                → tespit yöntemi, valgrind notu
 *   linked list                → dinamik struct allocation
 *   dynamic array              → realloc ile büyüyen dizi
 *   arena allocator            → basit custom allocator örneği
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "common.h"

/* ===========================================================================
 * BÖLÜM 1 — malloc, calloc, realloc, free farkları
 *
 * malloc(size)          → size byte ayırır, içerik başlatılmaz (garbage)
 * calloc(count, size)   → count*size byte ayırır, sıfırlar
 * realloc(ptr, newsize) → mevcut bloğu yeniden boyutlandırır
 *   - Büyütülürse yeni alan başlatılmaz
 *   - Başarısızlıkta NULL döner, orijinal ptr geçersiz değil
 *   - ptr NULL ise malloc gibi davranır
 *   - newsize 0 ise free gibi davranır (implementation-defined)
 * free(ptr)             → belleği serbest bırakır
 *   - NULL geçmek güvenli (no-op)
 *   - Aynı pointer'ı iki kez free → undefined behavior
 * ===========================================================================*/
static void demo_alloc_basics(void)
{
    /* malloc — içerik başlatılmaz */
    int *a = malloc(5 * sizeof(int));
    if (!a) { LOG_ERROR("malloc failed"); return; }

    for (int i = 0; i < 5; i++) a[i] = i + 1;
    printf("malloc: ");
    for (int i = 0; i < 5; i++) printf("%d ", a[i]);
    printf("\n");
    free(a);

    /* calloc — sıfırlanmış bellek */
    int *b = calloc(5, sizeof(int));
    if (!b) { LOG_ERROR("calloc failed"); return; }

    printf("calloc (zeroed): ");
    for (int i = 0; i < 5; i++) printf("%d ", b[i]);
    printf("\n");
    free(b);

    /* realloc — büyüyen dizi */
    int *c = malloc(3 * sizeof(int));
    if (!c) { LOG_ERROR("malloc failed"); return; }

    c[0] = 10; c[1] = 20; c[2] = 30;

    /* Kritik: realloc başarısız olursa NULL döner, orijinal ptr kaybolur.
     * Bu yüzden doğrudan c = realloc(c, ...) YANLIŞ — geçici pointer kullan */
    int *tmp = realloc(c, 6 * sizeof(int));
    if (!tmp) { LOG_ERROR("realloc failed"); free(c); return; }
    c = tmp;

    c[3] = 40; c[4] = 50; c[5] = 60;
    printf("realloc (extended): ");
    for (int i = 0; i < 6; i++) printf("%d ", c[i]);
    printf("\n");
    free(c);
}

/* ===========================================================================
 * BÖLÜM 2 — Safe wrapper'lar
 *
 * Production kodunda malloc/calloc/realloc her çağrısında NULL kontrolü
 * yazmak yerine wrapper fonksiyonlar kullanılır.
 *
 * exit(EXIT_FAILURE): gömülü sistemlerde alternatif recovery mekanizması
 * tercih edilebilir (watchdog reset, error handler vb.)
 * ===========================================================================*/
static void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        LOG_ERROR("malloc(%zu) failed", size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

static void *safe_calloc(size_t count, size_t size)
{
    void *ptr = calloc(count, size);
    if (!ptr)
    {
        LOG_ERROR("calloc(%zu, %zu) failed", count, size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

static void *safe_realloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr)
    {
        LOG_ERROR("realloc(%zu) failed", size);
        free(ptr);   /* orijinal bloğu temizle */
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

static void demo_safe_wrappers(void)
{
    int *arr = safe_malloc(4 * sizeof(int));
    for (int i = 0; i < 4; i++) arr[i] = i * 10;

    arr = safe_realloc(arr, 8 * sizeof(int));
    for (int i = 4; i < 8; i++) arr[i] = i * 10;

    printf("safe alloc: ");
    for (int i = 0; i < 8; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
}

/* ===========================================================================
 * BÖLÜM 3 — Dangling pointer ve double free
 *
 * Dangling pointer: free edilen belleğe işaret eden pointer.
 *   - Dereference → undefined behavior (crash veya sessiz veri bozulması)
 *   - Önlem: free sonrası ptr = NULL
 *
 * Double free: aynı pointer'ı iki kez free etme.
 *   - Heap corruption → crash veya güvenlik açığı
 *   - Önlem: free sonrası NULL atama (free(NULL) no-op)
 * ===========================================================================*/
static void demo_dangling_double_free(void)
{
    int *ptr = safe_malloc(sizeof(int));
    *ptr = 42;
    printf("before free: %d\n", *ptr);

    free(ptr);
    ptr = NULL;   /* dangling pointer önlemi */

    /* Güvenli kontrol */
    if (ptr != NULL)
    {
        printf("value: %d\n", *ptr);   /* NULL olduğu için buraya girmez */
    }
    else
    {
        printf("ptr is NULL — safe\n");
    }

    /* double free önlemi: NULL'ı free etmek güvenli (no-op) */
    free(ptr);   /* ptr == NULL → hiçbir şey yapmaz */
    free(ptr);   /* yine güvenli */
    printf("double free on NULL: safe\n");
}

/* ===========================================================================
 * BÖLÜM 4 — Memory leak
 *
 * Ayrılan bellek free edilmezse leak oluşur.
 * Uzun süre çalışan programlarda bellek tükenir.
 *
 * Tespit araçları:
 *   valgrind --leak-check=full ./program
 *   AddressSanitizer: gcc -fsanitize=address
 *   clang --analyze
 *
 * Yaygın leak kaynakları:
 *   - Erken return öncesi free unutmak
 *   - Hata yollarında free atlamak
 *   - Pointer'ı overwrite etmeden realloc yapmak
 * ===========================================================================*/
static char *load_string(const char *src)
{
    size_t len = strlen(src) + 1;
    char  *buf = safe_malloc(len);
    memcpy(buf, src, len);
    return buf;   /* çağıran free etmeli */
}

static void demo_memory_leak(void)
{
    /* Doğru kullanım — free ile temizleme */
    char *s1 = load_string("hello");
    char *s2 = load_string("world");

    printf("loaded: %s %s\n", s1, s2);

    /* Erken return simülasyonu — her iki pointer'ı da free et */
    free(s1); s1 = NULL;
    free(s2); s2 = NULL;

    /* Yaygın hata: pointer overwrite → önceki blok leak olur
     * char *p = malloc(10);
     * p = malloc(20);   ← 10 byte'lık blok kayboldu!
     * free(p);          ← sadece 20 byte'lık blok serbest */

    LOG_INFO("no leaks in this demo");
}

/* ===========================================================================
 * BÖLÜM 5 — Linked list (dinamik struct allocation)
 *
 * Her node heap'te ayrı ayrı tahsis edilir.
 * Serbest bırakma: liste başından sonuna, her node ayrı free edilmeli.
 * ===========================================================================*/
typedef struct Node
{
    int          value;
    struct Node *next;
} Node_t;

static Node_t *node_create(int value)
{
    Node_t *node = safe_malloc(sizeof(Node_t));
    node->value  = value;
    node->next   = NULL;
    return node;
}

static void list_push(Node_t **head, int value)
{
    Node_t *node = node_create(value);
    node->next   = *head;
    *head        = node;
}

static void list_print(const Node_t *head)
{
    printf("list: ");
    while (head)
    {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");
}

static void list_free(Node_t **head)
{
    Node_t *curr = *head;
    while (curr)
    {
        Node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    *head = NULL;   /* dangling önlemi */
}

static void demo_linked_list(void)
{
    Node_t *head = NULL;

    list_push(&head, 10);
    list_push(&head, 20);
    list_push(&head, 30);
    list_print(head);

    list_free(&head);
    printf("after free: head=%s\n", head ? "not null!" : "NULL (safe)");
}

/* ===========================================================================
 * BÖLÜM 6 — Dynamic array (realloc ile büyüyen dizi)
 *
 * C++'daki vector<int>'e benzer pattern.
 * Kapasite dolunca 2 katına çıkar (amortized O(1) insert).
 * ===========================================================================*/
typedef struct
{
    int    *data;
    size_t  size;
    size_t  capacity;
} DynArray_t;

static DynArray_t dynarray_create(void)
{
    return (DynArray_t){ .data = NULL, .size = 0, .capacity = 0 };
}

static void dynarray_push(DynArray_t *arr, int value)
{
    if (arr->size == arr->capacity)
    {
        size_t new_cap = (arr->capacity == 0) ? 4 : arr->capacity * 2;
        arr->data      = safe_realloc(arr->data, new_cap * sizeof(int));
        arr->capacity  = new_cap;
    }
    arr->data[arr->size++] = value;
}

static void dynarray_free(DynArray_t *arr)
{
    free(arr->data);
    arr->data     = NULL;
    arr->size     = 0;
    arr->capacity = 0;
}

static void demo_dynamic_array(void)
{
    DynArray_t arr = dynarray_create();

    for (int i = 1; i <= 10; i++)
        dynarray_push(&arr, i * 10);

    printf("dynarray (size=%zu cap=%zu): ", arr.size, arr.capacity);
    for (size_t i = 0; i < arr.size; i++) printf("%d ", arr.data[i]);
    printf("\n");

    dynarray_free(&arr);
}

/* ===========================================================================
 * BÖLÜM 7 — Arena allocator
 *
 * Tek büyük blok tahsis edilir, içinden sırayla küçük bloklar verilir.
 * reset() ile tek seferde tamamı serbest bırakılır.
 *
 * Avantajlar:
 *   - malloc/free overhead yok
 *   - Fragmentation yok
 *   - Cache dostu (lineer bellek)
 *
 * Dezavantaj:
 *   - Bireysel free yok
 *   - Ömür yönetimi manuel
 * ===========================================================================*/
typedef struct
{
    uint8_t *base;     /* arena başlangıcı */
    size_t   size;     /* toplam boyut */
    size_t   offset;   /* mevcut konum */
} Arena_t;

static Arena_t arena_create(size_t size)
{
    Arena_t a;
    a.base   = safe_malloc(size);
    a.size   = size;
    a.offset = 0;
    return a;
}

static void *arena_alloc(Arena_t *a, size_t size)
{
    /* 8-byte hizalama */
    size_t aligned = (size + 7) & ~(size_t)7;

    if (a->offset + aligned > a->size)
    {
        LOG_ERROR("arena out of memory");
        return NULL;
    }

    void *ptr = a->base + a->offset;
    a->offset += aligned;
    return ptr;
}

static void arena_reset(Arena_t *a)
{
    a->offset = 0;   /* offset sıfırla — bellek yeniden kullanılabilir */
}

static void arena_free(Arena_t *a)
{
    free(a->base);
    a->base   = NULL;
    a->size   = 0;
    a->offset = 0;
}

static void demo_arena(void)
{
    Arena_t arena = arena_create(1024);

    int   *nums = arena_alloc(&arena, 4 * sizeof(int));
    char  *str  = arena_alloc(&arena, 32);

    if (nums && str)
    {
        for (int i = 0; i < 4; i++) nums[i] = i + 1;
        strncpy(str, "arena string", 31);

        printf("arena nums: %d %d %d %d\n", nums[0], nums[1], nums[2], nums[3]);
        printf("arena str : %s\n", str);
        printf("arena used: %zu / %zu bytes\n", arena.offset, arena.size);
    }

    /* Reset — ayrılan bloklara erişim geçersiz hale gelir */
    arena_reset(&arena);
    printf("arena reset: offset=%zu\n", arena.offset);

    arena_free(&arena);
}

/* ===========================================================================
 * main
 * ===========================================================================*/
int main(void)
{
    printf(LINE);
    LOG_INFO("memory.c — starting demos");
    printf(LINE);

    demo_alloc_basics();       printf(LINE);
    demo_safe_wrappers();      printf(LINE);
    demo_dangling_double_free(); printf(LINE);
    demo_memory_leak();        printf(LINE);
    demo_linked_list();        printf(LINE);
    demo_dynamic_array();      printf(LINE);
    demo_arena();              printf(LINE);

    LOG_INFO("memory.c — all demos complete");
    return 0;
}