#include "AES.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

#define TraceNum    5000
#define PointNum    4984
// #define PointNum    1000
#define StartPoint  810
#define EndPoint    2915
#define KeyCandidateNum 256
#define PlainFileName   "plaintext.txt"
#define TraceFileName   "realPowerConsumption.txt"
#define ResultFileName  "CPAresult.txt"

void test1() {
    FILE *fp_pt, *fp_trace;   // for PlainFile, TraceFile
    FILE *fp_rst;   // for ResultFile

    fp_pt = fopen(PlainFileName, "r");
    if (fp_pt == NULL) {
        fprintf(stderr, "File Open Error\n");
    }

    fp_trace = fopen(TraceFileName, "r");
    if (fp_trace == NULL) {
        fprintf(stderr, "File Open Error\n");
    }
    
    fp_rst = fopen(ResultFileName, "w");
    if (fp_rst == NULL) {
        fprintf(stderr, "File Open Error\n");
    }


    //todo make plaintext table
    uint8_t** ptTable;      // ptTable[trace_cnt][16]
    ptTable = (uint8_t**)malloc(sizeof(uint8_t*) * TraceNum);
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        ptTable[trace_cnt] = (uint8_t*)malloc(sizeof(uint8_t) * 16);
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fscanf(fp_pt, "%2hhx", &ptTable[trace_cnt][cnt_i]);
        }
    }

    // for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
    //     // ptTable[trace_cnt] = (uint8_t*)malloc(sizeof(uint8_t) * 16);
    //     for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
    //         printf("%02x ", ptTable[trace_cnt][cnt_i]);
    //     }printf("\n");
    // }
    // exit(1);

    //todo read powerConsumption
    float** powerConsumptionValue;
    powerConsumptionValue = (float**)malloc(sizeof(float*) * TraceNum);
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        powerConsumptionValue[trace_cnt] = (float*)malloc(sizeof(float) * PointNum);
        
        for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
            fscanf(fp_trace, "%f", &powerConsumptionValue[trace_cnt][point_cnt]);
        }
    }

    // for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
    //     // ptTable[trace_cnt] = (uint8_t*)malloc(sizeof(uint8_t) * 16);
    //     for (size_t cnt_i = 0; cnt_i < PointNum; cnt_i++) {
    //         printf("%f ", powerConsumptionValue[trace_cnt][cnt_i]);
    //     }printf("\n");
    // }
    // exit(1);
    

    double ptHammingWeigTable[TraceNum] = { 0 };
    double powerConsumptionPoint[TraceNum] = { 0 };
    double coef[PointNum] = { 0 };
    double finalRatio = 0;
    uint8_t sbox_tmp = 0x00;

    COEF_CANDIDATE maxCoefCandid[KeyCandidateNum] = { {0,0} };    // 00~ff(guessKey)에서 각각의 Coef의 최댓값과 그때의 guessKey를 저장하는 구조체 배열

    for (size_t guessByteIndex = 0; guessByteIndex < 1; guessByteIndex++) {
        // initialize
        memset(ptHammingWeigTable, 0, sizeof(ptHammingWeigTable));
        memset(powerConsumptionPoint, 0, sizeof(powerConsumptionPoint));
        memset(maxCoefCandid, 0, sizeof(maxCoefCandid));
        memset(coef, 0, sizeof(coef));
        finalRatio = 0;

        for (size_t guessKey = 0; guessKey < KeyCandidateNum; guessKey++) {
            
            // 중간 값 추출
            for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
                sbox_tmp = ptTable[trace_cnt][guessByteIndex] ^ guessKey;
                sbox_tmp = Sbox[sbox_tmp];
                ptHammingWeigTable[trace_cnt] = hammingWeigTable[sbox_tmp];
                // ptHammingWeigTable[trace_cnt] = hammingWeigTable[Sbox[ptTable[trace_cnt][guessByteIndex] ^ guessKey]];
            }

            for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
                // 전력소비 값 추출
                for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {     
                    powerConsumptionPoint[trace_cnt] = powerConsumptionValue[trace_cnt][point_cnt];
                }

                coef[point_cnt] = pearsonCorrelationCoef(ptHammingWeigTable, powerConsumptionPoint, TraceNum);  // 상관계수 구하기
                // printf("%lf\n", coef[point_cnt]);
            }

            

            mergeSortArrayVer(coef, 0, PointNum - 1);
            maxCoefCandid[guessKey].value = coef[PointNum - 1];
            maxCoefCandid[guessKey].originalIndex = guessKey;
        }

        // for (int cnt_i = 0; cnt_i < 256; cnt_i++) {
        //     printf("%lf ", maxCoefCandid[cnt_i].value);
        //     if ((cnt_i + 1) % 16 == 0) {
        //         printf("\n");
        //     }
        // }
        // printf("\n\n");

        mergeSortStructVer(maxCoefCandid, 0, KeyCandidateNum - 1);
        finalRatio = maxCoefCandid[KeyCandidateNum - 1].value / maxCoefCandid[KeyCandidateNum - 2].value;   // 최댓값 / 두번째 최댓값으로 Ratio 구하기

        fprintf(fp_rst, "[%02d byte]\t %02X\t %lf\t %lf\n",
            guessByteIndex, maxCoefCandid[KeyCandidateNum - 1].originalIndex,
                maxCoefCandid[KeyCandidateNum - 1].value, finalRatio);
    }

    for (size_t cnt_i = 0; cnt_i < TraceNum; cnt_i++) {
        free(ptTable[cnt_i]);
    }
    free(ptTable);
    
    for (size_t cnt_i = 0; cnt_i < TraceNum; cnt_i++) {
        free(powerConsumptionValue[cnt_i]);
    }
    free(powerConsumptionValue);

    fclose(fp_pt);
    fclose(fp_trace);
    fclose(fp_rst);

    // return 0;
}

void test2() {
    FILE *fp_pt, *fp_trace;   // for PlainFile, TraceFile
    FILE *fp_rst;   // for ResultFile

    fp_pt = fopen(PlainFileName, "r");
    if (fp_pt == NULL) {
        fprintf(stderr, "File Open Error\n");
    }

    fp_trace = fopen(TraceFileName, "r");
    if (fp_trace == NULL) {
        fprintf(stderr, "File Open Error\n");
    }
    
    fp_rst = fopen(ResultFileName, "w");
    if (fp_rst == NULL) {
        fprintf(stderr, "File Open Error\n");
    }

    //todo read hamming weight of plaintext and make table


    //todo make plaintext table
    uint8_t** ptTable;      // ptTable[trace_cnt][16]
    ptTable = (uint8_t**)malloc(sizeof(uint8_t*) * TraceNum);
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        ptTable[trace_cnt] = (uint8_t*)malloc(sizeof(uint8_t) * 16);
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fscanf(fp_pt, "%2hhx", &ptTable[trace_cnt][cnt_i]);
        }
    }
    // uint8_t ptTable[TraceNum][16] = { 0x00 };
    // for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
    //     for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
    //         fscanf(fp_pt, "%2hhx", &ptTable[trace_cnt][cnt_i]);
    //     }
    // }

    //todo read powerConsumption
    float** powerConsumptionValue;      // powerConsumption[trace_cnt][point_cnt]
    powerConsumptionValue = (float**)malloc(sizeof(float*) * TraceNum);
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        powerConsumptionValue[trace_cnt] = (float*)malloc(sizeof(float) * PointNum);
        // memset(powerConsumptionValue[0], 0, sizeof(powerConsumptionValue[trace_cnt]));
        for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
            fscanf(fp_trace, "%f", &powerConsumptionValue[trace_cnt][point_cnt]);
        }
    }
    

    double ptHammingWeigTable[TraceNum] = { 0 };
    double powerConsumptionPoint[TraceNum] = { 0 };
    double coef[EndPoint - StartPoint + 1] = { 0 };
    double finalRatio = 0;

    COEF_CANDIDATE maxCoefCandid[KeyCandidateNum] = { {0,0} };    // 00~ff(guessKey)에서 각각의 Coef의 최댓값과 그때의 guessKey를 저장하는 구조체 배열

    for (size_t guessByteIndex = 0; guessByteIndex < 16; guessByteIndex++) {
        // initialize
        memset(maxCoefCandid, 0, sizeof(maxCoefCandid));    
        finalRatio = 0;

        for (size_t guessKey = 0; guessKey < KeyCandidateNum; guessKey++) {
            // 중간 값 추출
            for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
                ptHammingWeigTable[trace_cnt] = hammingWeigTable[Sbox[ptTable[trace_cnt][guessByteIndex] ^ guessKey]];
            }

            for (size_t point_cnt = StartPoint; point_cnt < EndPoint + 1; point_cnt++) {
                // 전력소비 값 추출
                for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {     
                    powerConsumptionPoint[trace_cnt] = powerConsumptionValue[trace_cnt][point_cnt];
                }

                coef[point_cnt - StartPoint] = pearsonCorrelationCoef(ptHammingWeigTable, powerConsumptionPoint, TraceNum);  // 상관계수 구하기
            }

            mergeSortArrayVer(coef, 0, (EndPoint - StartPoint + 1) - 1);
            maxCoefCandid[guessKey].value = coef[(EndPoint - StartPoint + 1) - 1];
            maxCoefCandid[guessKey].originalIndex = guessKey;
        }

        mergeSortStructVer(maxCoefCandid, 0, KeyCandidateNum - 1);
        finalRatio = maxCoefCandid[KeyCandidateNum - 1].value / maxCoefCandid[KeyCandidateNum - 2].value;   // 최댓값 / 두번째 최댓값으로 Ratio 구하기

        fprintf(fp_rst, "[%02d byte]\t %02X\t %lf\t %lf\n",
            guessByteIndex, maxCoefCandid[KeyCandidateNum - 1].originalIndex,
                maxCoefCandid[KeyCandidateNum - 1].value, finalRatio);
    }

    for (size_t cnt_i = 0; cnt_i < TraceNum; cnt_i++) {
        free(ptTable[cnt_i]);
    }
    free(ptTable);
    
    for (size_t cnt_i = 0; cnt_i < TraceNum; cnt_i++) {
        free(powerConsumptionValue[cnt_i]);
    }
    free(powerConsumptionValue);

    fclose(fp_pt);
    fclose(fp_trace);
    fclose(fp_rst);

    // return 0;
}

int main() {

    test1();
    // test2();

    return 0;
}
