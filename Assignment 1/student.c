#include "student.h"

int largest(int array[], int length) {
    int i;
    int largest = array[0];
    for (i = 0; i < length; i++) {
        if (largest < array[i]) {
            largest = array[i];
        }
    }
    return largest;
}

int sum(int array[], int length) {
    int i;
    int sum = 0;
    for (i = 0; i < length; i++) {
        sum += array[i];
    }
    return sum;
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void rotate(int *a, int *b, int *c) {
    int tmp = *a;
    *a = *c;
    *c = *b;
    *b = tmp;
}

void sort(int array[], int length) {
    int i;
    int j;
    for (i = 0; i < length - 1; i++) {
        for (j = 0; j < length - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
}

void double_primes(int array[], int length) {
    int i;
    int isPrime = 1;
    for (i = 0; i < length; i++) {
        isPrime = 1;
        for (int j = 2; j < array[i]; j++) {
            if (array[i] % j == 0 && array[i] > 2) {
                isPrime = 0;
                break;
            }
        }
        if (array[i] <= 1) {
            isPrime = 0;
        }
        if (isPrime == 1) {
            array[i] = array[i] * 2;
        }
    }
}

void negate_armstrongs(int array[], int length) {
    int i;
    int sum;
    int testNumber;
    int digits;
    int remainder;
    for (i = 0; i < length; i++) {
        digits = 0;
        sum = 0;
        remainder = 0;
        testNumber = array[i];
        for (testNumber = array[i]; testNumber != 0; digits++) {
            testNumber /= 10;
        }
        for (testNumber = array[i]; testNumber != 0; testNumber /= 10) {
            remainder = testNumber % 10;
            sum += power(remainder, digits);
        }
        if (sum == array[i] && array[i] >= 0) {
            array[i] = array[i] * -1;
        }
    }
}

int power(int n, int x) {
    if (x == 0) return 1;
    if (x % 2 == 0) {
        return power(n, x / 2) * power(n, x / 2);
    }
    return n * power(n, x / 2) * power(n, x / 2);
}