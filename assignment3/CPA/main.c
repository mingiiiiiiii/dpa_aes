#include "AES.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

#define TraceNum    5000
#define PointNum    4984
#define StartPoint  1600
#define EndPoint    3360
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

    //todo read powerConsumption
    float** powerConsumptionValue;
    powerConsumptionValue = (float**)malloc(sizeof(float*) * TraceNum);
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        powerConsumptionValue[trace_cnt] = (float*)malloc(sizeof(float) * PointNum);
        
        for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
            fscanf(fp_trace, "%f", &powerConsumptionValue[trace_cnt][point_cnt]);
        }
    }
    
    uint32_t ptHammingWeigTable[TraceNum] = { 0 };
    double pointPowerConsumptionValue[TraceNum] = { 0 };
    double pearsonCorrCoef[PointNum] = { 0 };
    double finalRatio = 0;
    uint8_t sbox_tmp = 0x00;

    PEARSON_CORR_CANDIDATE maxPearsonCorrCoefCandid[KeyCandidateNum] = { {0,0} };    // 00~ff(guessKey)에서 각각의 Coef의 최댓값과 그때의 guessKey를 저장하는 구조체 배열

    for (size_t guessByteIndex = 0; guessByteIndex < 16; guessByteIndex++) {
        // initialize
        memset(ptHammingWeigTable, 0, sizeof(ptHammingWeigTable));
        memset(pointPowerConsumptionValue, 0, sizeof(pointPowerConsumptionValue));
        memset(maxPearsonCorrCoefCandid, 0, sizeof(maxPearsonCorrCoefCandid));
        memset(pearsonCorrCoef, 0, sizeof(pearsonCorrCoef));
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
                    pointPowerConsumptionValue[trace_cnt] = powerConsumptionValue[trace_cnt][point_cnt];
                }

                pearsonCorrCoef[point_cnt] = calcPearsonCorrelationCoefficient(ptHammingWeigTable, pointPowerConsumptionValue, TraceNum);  // 상관계수 구하기
                // printf("%lf\n", pearsonCorrCoef[point_cnt]);
            }

            mergeSortArrayVer(pearsonCorrCoef, 0, PointNum - 1);
            maxPearsonCorrCoefCandid[guessKey].value = pearsonCorrCoef[PointNum - 1];
            maxPearsonCorrCoefCandid[guessKey].originalIndex = guessKey;
        }

        mergeSortStructVer(maxPearsonCorrCoefCandid, 0, KeyCandidateNum - 1);
        finalRatio = maxPearsonCorrCoefCandid[KeyCandidateNum - 1].value / maxPearsonCorrCoefCandid[KeyCandidateNum - 2].value;   // 최댓값 / 두번째 최댓값으로 Ratio 구하기

        fprintf(fp_rst, "[%02d byte]\t %02X\t %lf\t %lf\n",
            guessByteIndex, maxPearsonCorrCoefCandid[KeyCandidateNum - 1].originalIndex,
                maxPearsonCorrCoefCandid[KeyCandidateNum - 1].value, finalRatio);
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

    //todo make plaintext table
    uint8_t** ptTable;      // ptTable[trace_cnt][16]
    ptTable = (uint8_t**)malloc(sizeof(uint8_t*) * TraceNum);
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        ptTable[trace_cnt] = (uint8_t*)malloc(sizeof(uint8_t) * 16);
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fscanf(fp_pt, "%2hhx", &ptTable[trace_cnt][cnt_i]);
        }
    }
    
    //todo read powerConsumption
    float** powerConsumptionValue;      // powerConsumption[trace_cnt][point_cnt]
    powerConsumptionValue = (float**)malloc(sizeof(float*) * TraceNum);
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        powerConsumptionValue[trace_cnt] = (float*)malloc(sizeof(float) * PointNum);
        for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
            fscanf(fp_trace, "%f", &powerConsumptionValue[trace_cnt][point_cnt]);
        }
    }

    uint32_t ptHammingWeigTable[TraceNum] = { 0 };      // 실제 평문으로 만든 중간값의 해밍웨이트를 저장할 배열
    double pointPowerConsumptionValue[TraceNum] = { 0 };    // 포인트 별 소비전력을 저장할 배열
    double pearsonCorrCoef[EndPoint - StartPoint + 1] = { 0 };     // 포인트 별 상관계수를 저장할 배열
    double finalRatio = 0;

    PEARSON_CORR_CANDIDATE maxPearsonCorrCoefCandid[KeyCandidateNum] = { {0,0} };    // 00~ff(guessKey)에서 각각의 상관계수의 최댓값과 그때의 guessKey를 저장하는 구조체 배열

    for (size_t guessByteIndex = 0; guessByteIndex < 16; guessByteIndex++) {
        // initialize
        memset(ptHammingWeigTable, 0, sizeof(ptHammingWeigTable));
        memset(pointPowerConsumptionValue, 0, sizeof(pointPowerConsumptionValue));
        memset(maxPearsonCorrCoefCandid, 0, sizeof(maxPearsonCorrCoefCandid));
        memset(pearsonCorrCoef, 0, sizeof(pearsonCorrCoef));
        finalRatio = 0;

        for (size_t guessKey = 0; guessKey < KeyCandidateNum; guessKey++) {

            // 중간 값 추출
            for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
                ptHammingWeigTable[trace_cnt] = hammingWeigTable[Sbox[ptTable[trace_cnt][guessByteIndex] ^ guessKey]];
            }

            for (size_t point_cnt = StartPoint; point_cnt < EndPoint + 1; point_cnt++) {
                // 전력소비 값 추출
                for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {     
                    pointPowerConsumptionValue[trace_cnt] = powerConsumptionValue[trace_cnt][point_cnt];
                }

                // 상관계수 구하기
                pearsonCorrCoef[point_cnt - StartPoint] = calcPearsonCorrelationCoefficient(ptHammingWeigTable, pointPowerConsumptionValue, TraceNum);  // 상관계수 구하기
            }

            // 포인트 별 상관계수 중 최댓값을 추측 키의 대푯값으로 설정하기
            mergeSortArrayVer(pearsonCorrCoef, 0, (EndPoint - StartPoint + 1) - 1);
            maxPearsonCorrCoefCandid[guessKey].value = pearsonCorrCoef[(EndPoint - StartPoint + 1) - 1];
            maxPearsonCorrCoefCandid[guessKey].originalIndex = guessKey;
        }

        // 추측 키의 대푯값 들의 모임에서 최댓값 뽑기
        mergeSortStructVer(maxPearsonCorrCoefCandid, 0, KeyCandidateNum - 1);
        finalRatio = maxPearsonCorrCoefCandid[KeyCandidateNum - 1].value / maxPearsonCorrCoefCandid[KeyCandidateNum - 2].value;   // 최댓값 / 두번째 최댓값으로 Ratio 구하기

        fprintf(fp_rst, "[%02d byte]\t %02X\t %lf\t %lf\n",
            guessByteIndex, maxPearsonCorrCoefCandid[KeyCandidateNum - 1].originalIndex,
                maxPearsonCorrCoefCandid[KeyCandidateNum - 1].value, finalRatio);
    }

    // 메모리 해제
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

}

int main() {

    test1();     // 모든 포인트 분석
    // test2();        // 특정 구간 분석 (StartPoint, EndPoint)

    return 0;
}
