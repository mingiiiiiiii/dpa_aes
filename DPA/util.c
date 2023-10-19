#include "util.h"

uint8_t GetHammingWeight(uint8_t src) {
    uint8_t hamming_weight = 0;     // 8-bit에 대한 hammingweight 최댓값 8

    for (size_t cnt_i = 0; cnt_i < 8; cnt_i++) {
        hamming_weight += ((src >> cnt_i) & 0x01);  // 1인지 0인지 확인하기
    }

    return hamming_weight;
}

void MakeHammingWeightTable(uint8_t state[], uint32_t table[]) {
    for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
        table[cnt_i] = GetHammingWeight(state[cnt_i]);
    }
}



// int popcount_1(unsigned char in) {

// unsigned char m1  = 0x55;
// unsigned char m2  = 0x33;
// unsigned char m4  = 0x0f;
// unsigned char B,C = 0;
// unsigned char x = in;

// x = (x & (x << 1) & (m1 << 1)) | (m1 & (x ^ (x >> 1)));

// B = x & m2;
// C = (x >>  2) & m2;
// x = B ^ C ^ ((B & C) << 1);

// B = (x & m4 ) ^ ((x >>  4) & m4);
// C = (x & ((x >>  4) & m4)) << 1;
// x = B ^ C ^ ((B & C) << 1);
// return x;
// }

void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

int partition(double arr[], int low, int high) {
    double pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] > pivot) { // 부등호 방향 변경
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(double arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}