#include <stdio.h>
#include <stdint.h>

extern const uint8_t Sbox[256];

void addRoundKey(uint8_t state[], uint8_t roundkey[]);

void subBytes(uint8_t state[]);
void invSubBytes(uint8_t state[]);

void shiftRows(uint8_t state[]);
void InvShiftRows(uint8_t state[]);

void mixColumns(uint8_t state[]);
void invMixColumns(uint8_t state[]);

void keySchedule(uint8_t key[], uint8_t rk[11][16]);
