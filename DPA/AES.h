#include <stdio.h>
#include <stdint.h>

extern const uint8_t Sbox[256];

void AddRoundKey(uint8_t state[], uint8_t roundkey[]);

void SubBytes(uint8_t state[]);
void InvSubBytes(uint8_t state[]);

void ShiftRows(uint8_t state[]);
void InvShiftRows(uint8_t state[]);

void MixColumns(uint8_t state[]);
void InvMixColumns(uint8_t state[]);

void KeySchedule(uint8_t key[], uint8_t rk[11][16]);

void Print_Array(const uint8_t arr[], char* str);

void Encryption(uint8_t pt[], uint8_t rk[][16], uint8_t ct[], uint8_t key[]);
void Encryption_PrintVer(uint8_t pt[], uint8_t rk[][16], uint8_t ct[], uint8_t key[]);

void Decryption(uint8_t ct[], uint8_t rk[][16], uint8_t pt[]);

