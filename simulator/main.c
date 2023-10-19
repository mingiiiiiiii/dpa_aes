#include "AES.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

#define TraceNum    1000
#define PointNum    64
#define BufLen      1000
#define PlainFileName   "plaintext.txt"
#define KeyFileName     "masterKey.txt"
#define TraceFileName   "simulatedPowerConsumption.txt"
#define ResultFileName  "DPAresult.txt"

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

    uint32_t tmp = 0x00;    // for fscanf

    // Read Key
    for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
        fscanf(fp_key, "%X", &tmp);
        key[cnt_i] = tmp;
        // printf("%02X ", key[cnt_i]);
    }

    KeySchedule(key, roundKey);

    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        // Read PlainText
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fscanf(fp_pt, "%X", &tmp);
            state[cnt_i] = tmp;
        }

        //todo AES Round 1
        AddRoundKey(state, roundKey[0]);
        MakeHammingWeightTable(state, hammingWeig);
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        }

        SubBytes(state);
        MakeHammingWeightTable(state, hammingWeig);
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        }

        ShiftRows(state);
        MakeHammingWeightTable(state, hammingWeig);
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fprintf(fp_trace, "%d ", hammingWeig[cnt_i]);
        }

        MixColumns(state);
        MakeHammingWeightTable(state, hammingWeig);
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
