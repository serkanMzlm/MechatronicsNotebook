/*
 * algorithm_lab.c — Temel Algoritmalar
 *
 * Number theory, arama, sıralama ve sayı işlemleri örnekleri.
 */

#include <stdio.h>
#include <stdlib.h>

#include "algorithm_lab.h"
#include "common.h"

/* ===========================================================================
 * Number Theory
 * ===========================================================================*/

/* Basamaklarının küpü toplamı kendisine eşitse Armstrong */
int isArmstrongNumber(int n)
{
    int original = n, sum = 0;
    while (n > 0)
    {
        int d = n % 10;
        sum += d * d * d;
        n /= 10;
    }
    return sum == original;
}

/* Tersi kendisine eşitse palindrome */
int isPalindrome(int n)
{
    return n == reverseNumber(n);
}

/* Sayıyı basamak basamak tersine çevirir */
int reverseNumber(int n)
{
    int rev = 0;
    while (n > 0)
    {
        rev = rev * 10 + (n % 10);
        n /= 10;
    }
    return rev;
}

/* Onluk → ikili taban dönüşümü */
void decimalToBinary(int n)
{
    if (n == 0) { printf("Binary: 0\n"); return; }

    static int bits[MAX_BITS];
    int i = 0;

    while (n > 0 && i < MAX_BITS)
    {
        bits[i++] = n % 2;
        n /= 2;
    }

    printf("Binary: ");
    for (int j = i - 1; j >= 0; j--)   /* LSB → MSB ters basılır */
        printf("%d", bits[j]);
    printf("\n");
}

/* ===========================================================================
 * Factorial
 * ===========================================================================*/

/* n! = n * (n-1)! — klasik özyineleme */
int factorial(int n)
{
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

/* Çarpım zincirini yazdırır: 4 * 3 * 2 * 1 */
void printFactorial(int n)
{
    if (n <= 0) { printf("1"); return; }
    printf("%d", n);
    if (n > 1) printf(" * ");
    printFactorial(n - 1);
}

/* ===========================================================================
 * Fibonacci
 * ===========================================================================*/

/* İteratif — O(n) zaman, O(1) alan */
void fibonacci(int n)
{
    int a = 0, b = 1;
    for (int i = 0; i < n; i++)
    {
        printf("%d", a);
        if (i < n - 1) printf(", ");
        int next = a + b;
        a = b;
        b = next;
    }
    printf("\n");
}

/* Özyinelemeli helper — state pointer ile geçilir */
static void fib_helper(int n, int *a, int *b)
{
    if (n == 0) return;
    int next = *a + *b;
    printf("%d", *a);
    if (n > 1) printf(", ");
    *a = *b;
    *b = next;
    fib_helper(n - 1, a, b);
}

void fibonacciRecursion(int n)
{
    int a = 0, b = 1;
    fib_helper(n, &a, &b);
    printf("\n");
}

/* ===========================================================================
 * Search
 * ===========================================================================*/

/* Doğrusal arama — O(n), sırasız dizide çalışır */
int sequentialSearch(const int *arr, int size, int target)
{
    for (int i = 0; i < size; i++)
        if (arr[i] == target) return i;
    return -1;
}

/* İkili arama — O(log n), SIRALI dizi gerektirir */
int binarySearch(const int *arr, int left, int right, int target)
{
    while (left <= right)
    {
        int mid = left + (right - left) / 2;   /* taşma önlemi */
        if (arr[mid] == target) return mid;
        if (arr[mid] < target)  left  = mid + 1;
        else                    right = mid - 1;
    }
    return -1;
}

/* ===========================================================================
 * Sort
 * ===========================================================================*/

/* Yardımcı swap */
static void swap(int *a, int *b)
{
    int t = *a; *a = *b; *b = t;
}

/* Bubble sort — O(n²), her geçişte en büyük eleman sona taşınır */
void bubbleSort(int *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

/* Selection sort — O(n²), her geçişte en küçük eleman başa taşınır */
void selectionSort(int *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[min]) min = j;
        swap(&arr[i], &arr[min]);
    }
}

/* Insertion sort — O(n²), küçük ve neredeyse sıralı dizilerde verimli */
void insertionSort(int *arr, int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* ===========================================================================
 * Swap
 * ===========================================================================*/

/* Geçici değişken ile — en güvenli yöntem */
void swapTemp(int *a, int *b)
{
    int t = *a; *a = *b; *b = t;
}

/* XOR ile — geçici değişken yok, a==b ise 0 yapar (dikkat) */
void swapXOR(int *a, int *b)
{
    if (a == b) return;
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

/* ===========================================================================
 * main
 * ===========================================================================*/
static void print_arr(const char *label, const int *arr, int n)
{
    printf("%s: ", label);
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

int main(void)
{
    printf(LINE);
    LOG_INFO("algorithm_lab.c");
    printf(LINE);

    /* Number theory */
    int num = 153;
    decimalToBinary(num);
    printf("%d → %s Armstrong\n", num, isArmstrongNumber(num) ? "" : "Not");
    printf("%d → %s Palindrome\n", num, isPalindrome(num) ? "" : "Not");
    printf(LINE);

    /* Factorial */
    printf("5! = %d\n", factorial(5));
    printf("5! = "); printFactorial(5); printf("\n");
    printf(LINE);

    /* Fibonacci */
    printf("fib iter  : "); fibonacci(8);
    printf("fib recur : "); fibonacciRecursion(8);
    printf(LINE);

    /* Search */
    int arr[] = {3, 7, 1, 9, 4, 6, 2, 8, 5};
    int n = (int)(sizeof(arr) / sizeof(arr[0]));

    int idx = sequentialSearch(arr, n, 9);
    printf("sequential search(9): index=%d\n", idx);

    /* Binary search için önce sırala */
    int sorted[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int sidx = binarySearch(sorted, 0, n - 1, 7);
    printf("binary search(7): index=%d\n", sidx);
    printf(LINE);

    /* Sort */
    int b[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    int s[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    int ins[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};

    bubbleSort(b, n);    print_arr("bubble   ", b, n);
    selectionSort(s, n); print_arr("selection", s, n);
    insertionSort(ins,n);print_arr("insertion", ins, n);
    printf(LINE);

    /* Swap */
    int x = 10, y = 20;
    swapTemp(&x, &y); printf("swapTemp: x=%d y=%d\n", x, y);
    swapXOR(&x, &y);  printf("swapXOR:  x=%d y=%d\n", x, y);

    return 0;
}