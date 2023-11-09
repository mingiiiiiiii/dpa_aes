#include "util.h"

//*************************************************
// //todo generate hamming weight table for 00~FF
// void genHammingWeigTable() {
//     int hammingWeig = 0;
//     int hammigWeigTable[256] = { 0 };

//     for (size_t cnt_i = 0; cnt_i < 256; cnt_i++) {
//         hammingWeig = 0;
//         for (size_t cnt_j = 0; cnt_j < 8; cnt_j++) {
//             hammingWeig += ((cnt_i >> cnt_j) & 0x01);  // 1인지 0인지 확인하기
//         }
//         hammigWeigTable[cnt_i] = hammingWeig;
//     }

//     printf("const uint32_t hammingWeigTable[256] = {\n\t");
//     for (size_t cnt_i = 0; cnt_i < 256; cnt_i++) {
//         printf("%d, ", hammigWeigTable[cnt_i]);
//         if ((cnt_i + 1) % 16 == 0) {
//             printf("\n\t");
//         }
//     }
//     printf("};");
// }
//*************************************************

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

//*************************************************
// Efficient implementation of getting hamming weight

// const unsigned char m1 = 0x55; // binary: 01010101 
// const unsigned char m2 = 0x33; // binary: 00110011 
// const unsigned char m4 = 0x0f; // binary: 00001111 

// uint32_t efficientHammingWeig(unsigned char src) {
//    x = src;
//    x = (x & m1) + ((x >> 1) & m1); // put count of each 2 bits into those 2 bits
//    x = (x & m2) + ((x >> 2) & m2); // put count of each 4 bits into those 4 bits
//    x = (x & m4) + ((x >> 4) & m4); // put count of each 8 bits into those 8 bits
//    return x;
// }
//*************************************************

// get hamming weight from table
void getHammingWeig(uint8_t state[], uint32_t table[]) {
    for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
        table[cnt_i] = hammingWeigTable[state[cnt_i]];
    }
}

