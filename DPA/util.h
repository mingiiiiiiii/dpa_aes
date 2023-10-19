#include "AES.h"
#include <stdlib.h>

uint8_t GetHammingWeight(uint8_t src);
void MakeHammingWeightTable(uint8_t state[], uint32_t table[]);

void swap(double* a, double* b);

int partition(double arr[], int low, int high);

void quickSort(double arr[], int low, int high);