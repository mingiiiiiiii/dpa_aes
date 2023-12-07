#ifndef UTIL_H

#include "AES.h"

// uint32_t efficientHammingWeig(unsigned char src)

void getHammingWeig(uint8_t state[], uint32_t table[]);
extern const uint32_t hammingWeigTable[256];

#endif