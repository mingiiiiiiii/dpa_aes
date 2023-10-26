#include "util.h"

double getAbsoluteValue(double src) {
    if (src < 0) {
        return src * (-1);
    }
    return src;
}

// merge function using struct
void mergeStructVer(EPSILON_CANDIDATE arr[], uint32_t left, uint32_t mid, uint32_t right) {
    uint32_t n1 = mid - left + 1;
    uint32_t n2 = right - mid;
    EPSILON_CANDIDATE L[n1], R[n2];

    // copy data to temp array
    for (size_t cnt_i = 0; cnt_i < n1; cnt_i++) {
        L[cnt_i] = arr[left + cnt_i];
    }
    for (size_t cnt_i = 0; cnt_i < n2; cnt_i++) {
        R[cnt_i] = arr[mid + 1 + cnt_i];
    }

    uint32_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].value <= R[j].value) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// mergeSort function using struct
void mergeSortStructVer(EPSILON_CANDIDATE arr[], uint32_t left, uint32_t right) {
    if (left < right) {
        uint32_t mid = left + (right - left) / 2;

        mergeSortStructVer(arr, left, mid);
        mergeSortStructVer(arr, mid + 1, right);

        mergeStructVer(arr, left, mid, right);
    }
}

// merge function using array
void mergeArrayVer(double arr[], uint32_t left, uint32_t mid, uint32_t right) {
    uint32_t n1 = mid - left + 1;
    uint32_t n2 = right - mid;
    double L[n1], R[n2];

    // copy data to temp array
    for (size_t cnt_i = 0; cnt_i < n1; cnt_i++) {
        L[cnt_i] = arr[left + cnt_i];
    }
    for (size_t cnt_i = 0; cnt_i < n2; cnt_i++) {
        R[cnt_i] = arr[mid + 1 + cnt_i];
    }

    uint32_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// mergeSort function using array
void mergeSortArrayVer(double arr[], uint32_t left, uint32_t right) {
    if (left < right) {
        uint32_t mid = left + (right - left) / 2;

        mergeSortArrayVer(arr, left, mid);
        mergeSortArrayVer(arr, mid + 1, right);

        mergeArrayVer(arr, left, mid, right);
    }
}
