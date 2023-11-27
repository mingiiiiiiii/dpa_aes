#include "util.h"

// Hamming weight table for 00~FF
const uint32_t hammingWeigTable[256] = {
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};

// get hamming weight from table
void getHammingWeig(uint8_t state[], uint32_t table[]) {
    for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
        table[cnt_i] = hammingWeigTable[state[cnt_i]];
    }
}

double getAbsoluteValue(double src) {
    if (src < 0) {
        return src * (-1);
    }
    return src;
}

// merge function using struct
void mergeStructVer(PEARSON_CORR_CANDIDATE arr[], uint32_t left, uint32_t mid, uint32_t right) {
    uint32_t n1 = mid - left + 1;
    uint32_t n2 = right - mid;
    PEARSON_CORR_CANDIDATE L[n1], R[n2];

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
void mergeSortStructVer(PEARSON_CORR_CANDIDATE arr[], uint32_t left, uint32_t right) {
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

double calcPearsonCorrelationCoefficient(uint32_t *x, double *y, uint32_t n) {
    uint32_t xSum = 0;
    uint32_t xSqrSum = 0;
    double ySum = 0;
    double ySqrSum = 0;
    double xySum = 0;

    double covariance = 0;
    double xVar = 0;
    double yVar = 0;

    double coef = 0;

    for (size_t cnt_i = 0; cnt_i < n; cnt_i++) {
        xSum += x[cnt_i];
        xSqrSum += (x[cnt_i] * x[cnt_i]);
        ySum += y[cnt_i];
        ySqrSum += (y[cnt_i] * y[cnt_i]);
        xySum += (x[cnt_i] * y[cnt_i]);
    }
    
    covariance = ((double)n * xySum) - (xSum * ySum);
    xVar = (n * xSqrSum) - (xSum * xSum);
    yVar = (n * ySqrSum) - (ySum * ySum);

    // 분모가 0일때의 예외처리
    if (yVar == 0 || xVar == 0) {
        coef = 0;
    }
    else {
        coef = fabs(covariance / sqrt(xVar * yVar));
    }

    return coef;
}

/*
double calcPearsonCorrelationCoefficient(double *x, double *y, uint32_t n) {
    double xSum = 0;
    double xAvg = 0;
    double ySum = 0;
    double yAvg = 0;
    double *xDev = (double*)calloc(n, sizeof(double));
    double *yDev = (double*)calloc(n, sizeof(double));

    double xDevSqrSum = 0;
    double yDevSqrSum = 0;
    double xyDevSum = 0;

    double coef = 0;

    for (size_t cnt_i = 0; cnt_i < n; cnt_i++) {
        xSum += x[cnt_i];
        ySum += y[cnt_i];
    }

    xAvg = xSum / n;
    yAvg = ySum / n;

    // 편차 구하기
    for (size_t cnt_i = 0; cnt_i < n; cnt_i++) {
        xDev[cnt_i] = x[cnt_i] - xAvg;
        yDev[cnt_i] = y[cnt_i] - yAvg;
    }

    for (size_t cnt_i = 0; cnt_i < n; cnt_i++) {
        xDevSqrSum += (xDev[cnt_i] * xDev[cnt_i]);
        yDevSqrSum += (yDev[cnt_i] * yDev[cnt_i]);
        xyDevSum += (xDev[cnt_i] * yDev[cnt_i]);
    }
    
    printf("xyDevSum = %lf, xDevSqrSum = %lf, yDevSqrSum = %lf\n", xyDevSum, xDevSqrSum, yDevSqrSum);    

    if (xyDevSum == 0) {
        coef = 0;
    }
    else {
        coef = fabs(xyDevSum / sqrt(xDevSqrSum * yDevSqrSum));    
    }

    free(xDev);
    free(yDev);

    return coef;
}
*/
