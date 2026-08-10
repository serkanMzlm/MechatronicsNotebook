#ifndef ALGORITHM_LAB_H
#define ALGORITHM_LAB_H

#define MAX_BITS 32

/* --- Number Theory --- */
int  isArmstrongNumber(int n);
int  isPalindrome(int n);
int  reverseNumber(int n);
void decimalToBinary(int n);

/* --- Factorial --- */
int  factorial(int n);
void printFactorial(int n);

/* --- Fibonacci --- */
void fibonacci(int n);
void fibonacciRecursion(int n);

/* --- Search --- */
int sequentialSearch(const int *arr, int size, int target);
int binarySearch(const int *arr, int left, int right, int target);

/* --- Sort --- */
void bubbleSort(int *arr, int n);
void selectionSort(int *arr, int n);
void insertionSort(int *arr, int n);

/* --- Swap --- */
void swapTemp(int *a, int *b);
void swapXOR(int *a, int *b);

#endif /* ALGORITHM_LAB_H */