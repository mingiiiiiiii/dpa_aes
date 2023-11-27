#ifndef UTIL_H


#include "AES.h"
#include <stdlib.h>
#include <math.h>

extern const uint32_t hammingWeigTable[256];

typedef struct pearson_corr_candidate {
    double value;
    uint32_t originalIndex;
} PEARSON_CORR_CANDIDATE;

void getHammingWeig(uint8_t state[], uint32_t table[]);

double getAbsoluteValue(double src);

void mergeStructVer(PEARSON_CORR_CANDIDATE arr[], uint32_t left, uint32_t mid, uint32_t right);
void mergeSortStructVer(PEARSON_CORR_CANDIDATE arr[], uint32_t left, uint32_t right);

void mergeArrayVer(double arr[], uint32_t left, uint32_t mid, uint32_t right);
void mergeSortArrayVer(double arr[], uint32_t left, uint32_t right);

double calcPearsonCorrelationCoefficient(uint32_t *x, double *y, uint32_t n);

#endif
