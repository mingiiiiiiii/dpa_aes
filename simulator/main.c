#include "AES.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TraceNum    1000
#define PointNum    64
#define PlainFileName   "plaintext.txt"
#define KeyFileName     "masterKey.txt"
#define TraceFileName   "simulatedPowerConsumption.txt"
// #define ResultFileName  "DPAresult.txt"

int main() {
    FILE *fp_key, *fp_pt;   // for KeyFile, PlainFile
    FILE *fp_trace;   // for TraceFile

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
    
    uint8_t key[16] = { 0x00 };
    uint8_t roundKey[11][16] = { 0x00 };
    uint8_t state[16] = { 0x00 };
    uint32_t hammingWeig[16] = { 0x00 };

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
for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
    fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
}

        subBytes(state);
        getHammingWeig(state, hammingWeig);
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        }

        shiftRows(state);
        getHammingWeig(state, hammingWeig);
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        }

        mixColumns(state);
        getHammingWeig(state, hammingWeig);
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        }
        fprintf(fp_trace, "\n");
    }

    fclose(fp_key);
    fclose(fp_pt);
    fclose(fp_trace);

    return 0;
}
