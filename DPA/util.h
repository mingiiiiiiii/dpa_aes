#ifndef UTIL_H

#include "AES.h"
#include <stdlib.h>

typedef struct difference_candidate {
    double value;
    uint32_t originalIndex;
} DIFFERENCE_CANDIDATE;

double getAbsoluteValue(double src);

void mergeStructVer(DIFFERENCE_CANDIDATE arr[], uint32_t left, uint32_t mid, uint32_t right);
void mergeSortStructVer(DIFFERENCE_CANDIDATE arr[], uint32_t left, uint32_t right);

void mergeArrayVer(double arr[], uint32_t left, uint32_t mid, uint32_t right);
void mergeSortArrayVer(double arr[], uint32_t left, uint32_t right);

#endif