#include <stdio.h>
#include <stdlib.h>

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

// int main() {
//     double arr[64]; // 크기가 64인 double 배열을 생성

//     // 배열 초기화 (예시로 무작위 값으로 초기화)
//     for (int i = 0; i < 64; i++) {
//         arr[i] = (double)rand() / RAND_MAX; // 0에서 1 사이의 무작위 값으로 초기화
//     }

//     printf("원래 배열:\n");
//     for (int i = 0; i < 64; i++) {
//         printf("%lf ", arr[i]);
//     }

//     quickSort(arr, 0, 63);

//     printf("\n역순으로 정렬된 배열:\n");
//     for (int i = 0; i < 64; i++) {
//         printf("%lf ", arr[i]);
//     }

//     return 0;
// }