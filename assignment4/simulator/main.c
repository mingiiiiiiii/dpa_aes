#include "AES.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TraceNum    1000
#define PointNum    64
#define PlainFileName   "plaintext.txt"
#define KeyFileName     "masterKey.txt"
#define TraceFileName   "simulatedPowerConsumption.txt"
#define CipherFileName  "ciphertext.txt"

// shuffle이 있어야함
// shuffle(arr);
// state[arr[i]] = Sbox[state[arr[i]]]

// random array
uint8_t rand_arr[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

void shuffle(uint8_t arr[]) {
    int j = -1;
    uint8_t tmp;
    for (size_t i = 15; i > 0; i--) {
        j = rand() % (i + 1);       // j <-- 0 <= j <= i인 임의의 정수
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

int main() {
    FILE *fp_key, *fp_pt;   // for KeyFile, PlainFile
    FILE *fp_trace;   // for TraceFile
    FILE *fp_ct;

    fp_pt = fopen(PlainFileName, "r");
    if (fp_pt == NULL) {
        fprintf(stderr, "File Open Error\n");
    }

    fp_key = fopen(KeyFileName, "r");
    if (fp_key == NULL) {
        fprintf(stderr, "File Open Error\n");
    }

    fp_trace = fopen(TraceFileName, "w");
    if (fp_trace == NULL) {
        fprintf(stderr, "File Open Error\n");
    }

    fp_ct = fopen(CipherFileName, "w");
    if (fp_ct == NULL) {
        fprintf(stderr, "File Open Error\n");
    }

    uint8_t key[16] = { 0x00 };
    uint8_t roundKey[11][16] = { 0x00 };
    uint8_t state[16] = { 0x00 };
    uint32_t hammingWeig[16] = { 0x00 };

    srand(time(NULL));  // seed 

    // read key from file
    for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
        fscanf(fp_key, "%2hhx", &key[cnt_i]);
    }

    keySchedule(key, roundKey);

    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        // read plaintext from file
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fscanf(fp_pt, "%2hhx", &state[cnt_i]);
        }

        //todo AES Round 1
        addRoundKey(state, roundKey[0]);
        getHammingWeig(state, hammingWeig);
        // for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
        //     fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        // }

        // subBytes(state);
        // my_subBytes(state);

        //     printf("before shuffle : ");
        //     for (size_t i = 0; i < 16; i++) {
        //         printf("%02X ", rand_arr[i]);
        //     } printf("\n");

        shuffle(rand_arr);

        //     printf("after shuffle : ");
        //     for (size_t i = 0; i < 16; i++) {
        //         printf("%02X ", rand_arr[i]);
        //     } printf("\n\n");

        // computes sbox and get hamming weight
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            state[rand_arr[cnt_i]] = Sbox[state[rand_arr[cnt_i]]];
            hammingWeig[cnt_i] = hammingWeigTable[state[rand_arr[cnt_i]]];
            fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        }

        shiftRows(state);
        getHammingWeig(state, hammingWeig);
        // for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
        //     fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        // }

        mixColumns(state);
        getHammingWeig(state, hammingWeig);
        // for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
        //     fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        // }
        fprintf(fp_trace, "\n");

        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fprintf(fp_ct, "%02X ", state[cnt_i]);
        }
        fprintf(fp_ct, "\n");
    }

    fclose(fp_key);
    fclose(fp_pt);
    fclose(fp_trace);
    fclose(fp_ct);

    return 0;
}
