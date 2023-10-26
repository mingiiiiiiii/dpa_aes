#include "AES.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

#define TraceNum    1000
#define PointNum    64
#define KeyCandidateNum 256
#define PlainFileName   "plaintext.txt"
#define TraceFileName   "simulatedPowerConsumption.txt"
#define ResultFileName  "DPAresult.txt"

int main() {
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
    uint32_t ptHammingWeigTable[TraceNum][PointNum] = { 0x00 };
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
            fscanf(fp_trace, "%d", &ptHammingWeigTable[trace_cnt][point_cnt]);
        }
    }

    //todo make plaintext table
    uint8_t ptTable[TraceNum][16] = { 0x00 };
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fscanf(fp_pt, "%2hhx", &ptTable[trace_cnt][cnt_i]);
        }
    }

    uint8_t pt_tmp = 0x00;    // 평문 특정 바이트 값을 위한 임시변수
    uint32_t msbZeroCnt, msbOneCnt;
    uint32_t msbZeroAcc[PointNum], msbOneAcc[PointNum];     // 해밍웨이트 누적 합을 저장하는 배열 (MSB에 따른 분류)
    double msbZeroAvg[PointNum], msbOneAvg[PointNum], epsilon[PointNum];    // 포인트들의 해밍웨이트 평균 값 (MSB에 따른 분류), 평균들의 차를 저장하는 배열
    double finalRatio = 0;

    EPSILON_CANDIDATE maxEpsilonCandid[256] = { {0,0} };    // 00~ff(guessKey)에서 각각의 epsilon에서의 최댓값과 그때의 guessKey를 저장하는 구조체 배열

    for (size_t guessByteIndex = 0; guessByteIndex < 16; guessByteIndex++) {
        // initialize
        memset(&maxEpsilonCandid, 0, sizeof(EPSILON_CANDIDATE));
        finalRatio = 0;

        for (size_t guessKey = 0; guessKey < 256; guessKey++) {
            // initialize
            msbZeroCnt = 0, msbOneCnt = 0;
            memset(msbZeroAcc, 0, sizeof(msbZeroAcc));
            memset(msbOneAcc, 0, sizeof(msbOneAcc));
            memset(msbZeroAvg, 0, sizeof(msbZeroAvg));
            memset(msbOneAvg, 0, sizeof(msbOneAvg));
            memset(epsilon, 0, sizeof(epsilon));

            for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
                //todo 중간 값 만들고 전력소비 값과 관계 선택
                pt_tmp = ptTable[trace_cnt][guessByteIndex];
                pt_tmp = Sbox[(pt_tmp ^ guessKey)];

                if (((pt_tmp >> 7) & 0x01) == 0) {   // MSB == 0
                    for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
                        msbZeroAcc[point_cnt] += ptHammingWeigTable[trace_cnt][point_cnt];
                    }
                    msbZeroCnt++;
                }
                else if (((pt_tmp >> 7) & 0x01) == 1) {  // MSB == 1
                    for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
                        msbOneAcc[point_cnt] += ptHammingWeigTable[trace_cnt][point_cnt];
                    }
                    msbOneCnt++;
                }
            }
            
            //todo 포인트 별 평균 구하고 & 평균들의 차를 계산하고 (절댓값) & 최댓값 뽑기
            for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
                msbZeroAvg[point_cnt] = (double)msbZeroAcc[point_cnt] / msbZeroCnt;
                msbOneAvg[point_cnt] = (double)msbOneAcc[point_cnt] / msbOneCnt;
                epsilon[point_cnt] = getAbsoluteValue(msbOneAvg[point_cnt] - msbZeroAvg[point_cnt]);
            }
            
            mergeSortArrayVer(epsilon, 0, 64 - 1);  // sort epsilon 
            maxEpsilonCandid[guessKey].originalIndex = guessKey; // struct --> [value, index(guessKey)]
            maxEpsilonCandid[guessKey].value = epsilon[64 - 1];  // 최댓값 대입 (포인트 64개 중 최댓값을 현재 loop의 guesskey의 대푯값으로 생각하기)
        }

        //todo 256개의 epsilon 최댓값으로 guesskey 확정하기
        mergeSortStructVer(maxEpsilonCandid, 0, KeyCandidateNum - 1);    // 대푯값이 모인 배열에서 value기준으로 정렬, struct --> [value, index(key)]
        finalRatio = maxEpsilonCandid[KeyCandidateNum - 1].value / maxEpsilonCandid[KeyCandidateNum - 2].value;   // 최댓값 / 두번째 최댓값으로 Ratio 구하기

        fprintf(fp_rst, "[%02d byte]\t %02X\t %lf\t %lf\n",
            guessByteIndex, maxEpsilonCandid[KeyCandidateNum - 1].originalIndex,
                maxEpsilonCandid[KeyCandidateNum - 1].value, finalRatio);
    }

    fclose(fp_pt);
    fclose(fp_trace);
    fclose(fp_rst);

    return 0;
}
