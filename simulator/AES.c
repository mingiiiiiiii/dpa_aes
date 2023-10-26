#define _CRT_SECURE_NO_WARNINGS

#include "AES.h"

const uint8_t Sbox[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint8_t InvSbox[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

uint8_t Rcon[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };

#define xtime(x) ((x) << 1) ^ ((((x) >> 7) & 1) * 0x1b)

//mixcolumn 행렬 곱셉부분이용

#define mul_02(x) xtime((x))
#define mul_03(x) mul_02((x)) ^ (x)
#define mul_04(x) mul_02(mul_02((x)))
#define mul_08(x) mul_02(mul_04((x)))
#define mul_09(x) mul_08((x)) ^ (x)
#define mul_0b(x) mul_08((x)) ^ mul_03((x))
#define mul_0d(x) mul_08((x)) ^ mul_04((x)) ^ (x)
#define mul_0e(x) mul_08((x)) ^ mul_04((x)) ^ mul_02((x))

//라운드 키의 state를 각 바이트별로 xor    u32 rk[round][4] = roundkey // 16바이트
void addRoundKey(uint8_t state[], uint8_t roundkey[]) {
	state[0] ^= roundkey[0];
	state[1] ^= roundkey[1];
	state[2] ^= roundkey[2];
	state[3] ^= roundkey[3];
	state[4] ^= roundkey[4];
	state[5] ^= roundkey[5];
	state[6] ^= roundkey[6];
	state[7] ^= roundkey[7];
	state[8] ^= roundkey[8];
	state[9] ^= roundkey[9];
	state[10] ^= roundkey[10];
	state[11] ^= roundkey[11];
	state[12] ^= roundkey[12];
	state[13] ^= roundkey[13];
	state[14] ^= roundkey[14];
	state[15] ^= roundkey[15];
	
}

// 각 바이트를 S-box를 참조하여 치환.
void subBytes(uint8_t state[]) {
	state[0] = Sbox[state[0]];
	state[1] = Sbox[state[1]];
	state[2] = Sbox[state[2]];
	state[3] = Sbox[state[3]];
	state[4] = Sbox[state[4]];
	state[5] = Sbox[state[5]];
	state[6] = Sbox[state[6]];
	state[7] = Sbox[state[7]];
	state[8] = Sbox[state[8]];
	state[9] = Sbox[state[9]];
	state[10] = Sbox[state[10]];
	state[11] = Sbox[state[11]];
	state[12] = Sbox[state[12]];
	state[13] = Sbox[state[13]];
	state[14] = Sbox[state[14]];
	state[15] = Sbox[state[15]];
}

void invSubBytes(uint8_t state[]) {
	state[0] = InvSbox[state[0]];
	state[1] = InvSbox[state[1]];
	state[2] = InvSbox[state[2]];
	state[3] = InvSbox[state[3]];
	state[4] = InvSbox[state[4]];
	state[5] = InvSbox[state[5]];
	state[6] = InvSbox[state[6]];
	state[7] = InvSbox[state[7]];
	state[8] = InvSbox[state[8]];
	state[9] = InvSbox[state[9]];
	state[10] = InvSbox[state[10]];
	state[11] = InvSbox[state[11]];
	state[12] = InvSbox[state[12]];
	state[13] = InvSbox[state[13]];
	state[14] = InvSbox[state[14]];
	state[15] = InvSbox[state[15]];
}

void shiftRows(uint8_t state[]) {
	uint8_t tmp1, tmp2;
	tmp1 = state[1];
	state[1] = state[5];  state[5] = state[9];
	state[9] = state[13]; state[13] = tmp1;

	tmp1 = state[2]; tmp2 = state[6];
	state[2] = state[10]; state[10] = tmp1;
	state[6] = state[14]; state[14] = tmp2;

	tmp1 = state[3];
	state[3] = state[15]; state[15] = state[11];
	state[11] = state[7]; state[7] = tmp1;
}

void InvShiftRows(uint8_t state[]) {
	uint8_t tmp1;
	uint8_t tmp2;

	tmp1 = state[1];
	state[1] = state[13]; state[13] = state[9];
	state[9] = state[5];  state[5] = tmp1;

	tmp1 = state[2]; tmp2 = state[6];
	state[2] = state[10]; state[10] = tmp1;
	state[6] = state[14]; state[14] = tmp2;

	tmp1 = state[3];
	state[3] = state[7];   state[7] = state[11];
	state[11] = state[15]; state[15] = tmp1;
}

//위에서 mixcolumn계산을 위해 구해논 값으로 mixcolumn 연산을 합니다.
void mixColumns(uint8_t state[]) {
	/*for (uint8_t i = 0; i < 4; i++) {
		uint8_t tmp1 = state[i * 4];   uint8_t b = state[i * 4 + 1];
		uint8_t tmp3 = state[i * 4 + 2]; uint8_t d = state[i * 4 + 3];
		for (uint8_t j = 0; j < 4; j++) {
			if (j == 0) state[i * 4 + 0] = mul_02(tmp1) ^ mul_03(b) ^ tmp3 ^ d;
			if (j == 1) state[i * 4 + 1] = tmp1 ^ mul_02(b) ^ mul_03(tmp3) ^ d;
			if (j == 2) state[i * 4 + 2] = tmp1 ^ b ^ mul_02(tmp3) ^ mul_03(d);
			if (j == 3) state[i * 4 + 3] = mul_03(tmp1) ^ b ^ tmp3 ^ mul_02(d);
		}
	}*/

    uint8_t tmp1 = 0x00;
    uint8_t tmp2 = 0x00;
    uint8_t tmp3 = 0x00;
    uint8_t tmp4 = 0x00;

	tmp1 = state[0];
	tmp2 = state[1];
	tmp3 = state[2];
	tmp4 = state[3];

	state[0] = mul_02(tmp1) ^ mul_03(tmp2) ^ tmp3 ^ tmp4;
	state[1] = tmp1 ^ mul_02(tmp2) ^ mul_03(tmp3) ^ tmp4;
	state[2] = tmp1 ^ tmp2 ^ mul_02(tmp3) ^ mul_03(tmp4);
	state[3] = mul_03(tmp1) ^ tmp2 ^ tmp3 ^ mul_02(tmp4);

	tmp1 = state[4];
	tmp2 = state[5];
	tmp3 = state[6];
	tmp4 = state[7];

	state[4] = mul_02(tmp1) ^ mul_03(tmp2) ^ tmp3 ^ tmp4;
	state[5] = tmp1 ^ mul_02(tmp2) ^ mul_03(tmp3) ^ tmp4;
	state[6] = tmp1 ^ tmp2 ^ mul_02(tmp3) ^ mul_03(tmp4);
	state[7] = mul_03(tmp1) ^ tmp2 ^ tmp3 ^ mul_02(tmp4);

	tmp1 = state[8];
	tmp2 = state[9];
	tmp3 = state[10];
	tmp4 = state[11];

	state[8] = mul_02(tmp1) ^ mul_03(tmp2) ^ tmp3 ^ tmp4;
	state[9] = tmp1 ^ mul_02(tmp2) ^ mul_03(tmp3) ^ tmp4;
	state[10] = tmp1 ^ tmp2 ^ mul_02(tmp3) ^ mul_03(tmp4);
	state[11] = mul_03(tmp1) ^ tmp2 ^ tmp3 ^ mul_02(tmp4);

	tmp1 = state[12];
	tmp2 = state[13];
	tmp3 = state[14];
	tmp4 = state[15];

	state[12] = mul_02(tmp1) ^ mul_03(tmp2) ^ tmp3 ^ tmp4;
	state[13] = tmp1 ^ mul_02(tmp2) ^ mul_03(tmp3) ^ tmp4;
	state[14] = tmp1 ^ tmp2 ^ mul_02(tmp3) ^ mul_03(tmp4);
	state[15] = mul_03(tmp1) ^ tmp2 ^ tmp3 ^ mul_02(tmp4);

}

void invMixColumns(uint8_t state[]) {
	/*for (int i = 0; i < 4; i++) {
		uint8_t tmp1 = arr[i * 4];   uint8_t b = arr[i * 4 + 1];
		uint8_t tmp3 = arr[i * 4 + 2]; uint8_t d = arr[i * 4 + 3];
		for (int j = 0; j < 4; j++) {
			if (j == 0) arr[i * 4 + 0] = mul_0e(tmp1) ^ mul_0b(b) ^ mul_0d(tmp3) ^ mul_09(d);
			if (j == 1) arr[i * 4 + 1] = mul_09(tmp1) ^ mul_0e(b) ^ mul_0b(tmp3) ^ mul_0d(d);
			if (j == 2) arr[i * 4 + 2] = mul_0d(tmp1) ^ mul_09(b) ^ mul_0e(tmp3) ^ mul_0b(d);
			if (j == 3) arr[i * 4 + 3] = mul_0b(tmp1) ^ mul_0d(b) ^ mul_09(tmp3) ^ mul_0e(d);
		}
	}*/

    uint8_t tmp1 = 0x00;
    uint8_t tmp2 = 0x00;
    uint8_t tmp3 = 0x00;
    uint8_t tmp4 = 0x00;

	tmp1 = state[0];
	tmp2 = state[1];
	tmp3 = state[2];
	tmp4 = state[3];

	state[0] = mul_0e(tmp1) ^ mul_0b(tmp2) ^ mul_0d(tmp3) ^ mul_09(tmp4);
	state[1] = mul_09(tmp1) ^ mul_0e(tmp2) ^ mul_0b(tmp3) ^ mul_0d(tmp4);
	state[2] = mul_0d(tmp1) ^ mul_09(tmp2) ^ mul_0e(tmp3) ^ mul_0b(tmp4);
	state[3] = mul_0b(tmp1) ^ mul_0d(tmp2) ^ mul_09(tmp3) ^ mul_0e(tmp4);

	tmp1 = state[4];
	tmp2 = state[5];
	tmp3 = state[6];
	tmp4 = state[7];

	state[4] = mul_0e(tmp1) ^ mul_0b(tmp2) ^ mul_0d(tmp3) ^ mul_09(tmp4);
	state[5] = mul_09(tmp1) ^ mul_0e(tmp2) ^ mul_0b(tmp3) ^ mul_0d(tmp4);
	state[6] = mul_0d(tmp1) ^ mul_09(tmp2) ^ mul_0e(tmp3) ^ mul_0b(tmp4);
	state[7] = mul_0b(tmp1) ^ mul_0d(tmp2) ^ mul_09(tmp3) ^ mul_0e(tmp4);

	tmp1 = state[8];
	tmp2 = state[9];
	tmp3 = state[10];
	tmp4 = state[11];

	state[8] = mul_0e(tmp1) ^ mul_0b(tmp2) ^ mul_0d(tmp3) ^ mul_09(tmp4);
	state[9] = mul_09(tmp1) ^ mul_0e(tmp2) ^ mul_0b(tmp3) ^ mul_0d(tmp4);
	state[10] = mul_0d(tmp1) ^ mul_09(tmp2) ^ mul_0e(tmp3) ^ mul_0b(tmp4);
	state[11] = mul_0b(tmp1) ^ mul_0d(tmp2) ^ mul_09(tmp3) ^ mul_0e(tmp4);

	tmp1 = state[12];
	tmp2 = state[13];
	tmp3 = state[14];
	tmp4 = state[15];

	state[12] = mul_0e(tmp1) ^ mul_0b(tmp2) ^ mul_0d(tmp3) ^ mul_09(tmp4);
	state[13] = mul_09(tmp1) ^ mul_0e(tmp2) ^ mul_0b(tmp3) ^ mul_0d(tmp4);
	state[14] = mul_0d(tmp1) ^ mul_09(tmp2) ^ mul_0e(tmp3) ^ mul_0b(tmp4);
	state[15] = mul_0b(tmp1) ^ mul_0d(tmp2) ^ mul_09(tmp3) ^ mul_0e(tmp4);

}

void keySchedule(uint8_t key[], uint8_t rk[11][16]) { //[0,1,2,,16] //[0,1,2,3]
	
	for (int i = 0; i < 16; i++) {
		rk[0][i] = key[i];
	}

	for (int i = 0; i < 10; i++) {
        uint8_t word[4] = { 0x00 };
        uint8_t tmp[4] = { 0x00 };
        //Rotword
		word[0] = rk[i][13];
		word[1] = rk[i][14];
		word[2] = rk[i][15];
		word[3] = rk[i][12];
		
		//SubWord
		tmp[0] = Sbox[word[0]] ^ Rcon[i]; //Rcon
		tmp[1] = Sbox[word[1]];
		tmp[2] = Sbox[word[2]];
		tmp[3] = Sbox[word[3]];

		rk[i + 1][0] = rk[i][0] ^ tmp[0];
		rk[i + 1][1] = rk[i][1] ^ tmp[1];
		rk[i + 1][2] = rk[i][2] ^ tmp[2];
		rk[i + 1][3] = rk[i][3] ^ tmp[3];
		rk[i + 1][4] = rk[i][4] ^ rk[i + 1][0];
		rk[i + 1][5] = rk[i][5] ^ rk[i + 1][1];
		rk[i + 1][6] = rk[i][6] ^ rk[i + 1][2];
		rk[i + 1][7] = rk[i][7] ^ rk[i + 1][3];
		rk[i + 1][8] = rk[i][8] ^ rk[i + 1][4];
		rk[i + 1][9] = rk[i][9] ^ rk[i + 1][5];
		rk[i + 1][10] = rk[i][10] ^ rk[i + 1][6];
		rk[i + 1][11] = rk[i][11] ^ rk[i + 1][7];
		rk[i + 1][12] = rk[i][12] ^ rk[i + 1][8];
		rk[i + 1][13] = rk[i][13] ^ rk[i + 1][9];
		rk[i + 1][14] = rk[i][14] ^ rk[i + 1][10];
		rk[i + 1][15] = rk[i][15] ^ rk[i + 1][11];	
	}
}

