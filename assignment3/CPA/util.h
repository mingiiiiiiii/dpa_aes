#ifndef UTIL_H


#include "AES.h"
#include <stdlib.h>
#include <math.h>

extern const double hammingWeigTable[256];

typedef struct coef_candidate {
    double value;
    uint32_t originalIndex;
} COEF_CANDIDATE;

void getHammingWeig(uint8_t state[], uint32_t table[]);

double getAbsoluteValue(double src);

void mergeStructVer(COEF_CANDIDATE arr[], uint32_t left, uint32_t mid, uint32_t right);
void mergeSortStructVer(COEF_CANDIDATE arr[], uint32_t left, uint32_t right);

void mergeArrayVer(double arr[], uint32_t left, uint32_t mid, uint32_t right);
void mergeSortArrayVer(double arr[], uint32_t left, uint32_t right);

double pearsonCorrelationCoef(double *x, double *y, uint32_t n);

#endif