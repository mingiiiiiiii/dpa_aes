#include "AES.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

#define TraceNum    1000
#define PointNum    64
#define BufLen      1000
#define PlainFileName   "plaintext.txt"
#define TraceFileName   "simulatedPowerConsumption.txt"
#define ResultFileName  "DPAresult.txt"

double GetAbsoluteValue(double src) {
    if (src < 0) {
        return src * (-1);
    }
    return src;
}

int GetMaxValueIndex(double src[], int size) {
    double max = src[0];
    int max_idx = 0;
    for (size_t cnt_i = 1; cnt_i < size; cnt_i++) {
        if (max < src[cnt_i]) {
            max = src[cnt_i];
            max_idx = cnt_i;
        }
    }
    return max_idx;
}

int GetSecondValueIndex(double src[], int size) {
    double max = src[0];
    double secondMax = src[0];

    int secondMax_idx = 0;

    for (size_t cnt_i = 1; cnt_i < size; cnt_i++) {
        if (src[cnt_i] > max) {
            secondMax = max;
            max = src[cnt_i];
        }
        else if ((src[cnt_i] > secondMax) && (src[cnt_i] < max)) {
            secondMax = src[cnt_i];
            secondMax_idx = cnt_i;
        }
    }

    return secondMax_idx;
}

double GetMaxValue(double src[], int size) {
    double max = src[0];
    // int max_idx = 0;
    for (size_t cnt_i = 1; cnt_i < size; cnt_i++) {
        if (max < src[cnt_i]) {
            max = src[cnt_i];
            // max_idx = cnt_i;
        }
    }
    return max;
}

double GetSecondValue(double src[], int size) {
    double max = src[0];
    double secondMax = src[0];

    // int secondMax_idx = 0;

    for (size_t cnt_i = 1; cnt_i < size; cnt_i++) {
        if (src[cnt_i] > max) {
            secondMax = max;
            max = src[cnt_i];
        }
        else if ((src[cnt_i] > secondMax) && (src[cnt_i] < max)) {
            secondMax = src[cnt_i];
            // secondMax_idx = cnt_i;
        }
    }

    // return secondMax_idx;
    return secondMax;
}



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

    //todo Make Hamming Weight Table
    uint32_t hammingWeigTable[TraceNum][PointNum] = { 0x00 };
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
            fscanf(fp_trace, "%d", &hammingWeigTable[trace_cnt][point_cnt]);
        }
    }

    //todo Make PlainText Table
    uint8_t ptTable[TraceNum][16] = { 0x00 };
    uint32_t tmp1 = 0x00;    // for fscanf
    for (size_t trace_cnt = 0; trace_cnt < TraceNum; trace_cnt++) {
        for (size_t cnt_i = 0; cnt_i < 16; cnt_i++) {
            fscanf(fp_pt, "%X", &tmp1);
            ptTable[trace_cnt][cnt_i] = tmp1;
        }
    }
    //! pass

    uint8_t pt_tmp = 0x00;    // for 평문 특정 바이트 값
    uint32_t msbZeroCnt, msbOneCnt;
    uint32_t msbZeroAcc[PointNum], msbOneAcc[PointNum];
    double msbZeroAvg[PointNum], msbOneAvg[PointNum], epsilon[PointNum];
    double maxEpsilonCandid[256][2];    // for Ratio
    uint8_t finalGuessKey[16] = { 0x00 };
    double finalRatio[16] = { 0x00 };
    int max_idx;
    double tmptmp[256] = { 0x00 };


    for (size_t guessByteIndex = 0; guessByteIndex < 16; guessByteIndex++) {
        memset(maxEpsilonCandid, 0, sizeof(maxEpsilonCandid));
        max_idx = 0;
        //! have to change
        memset(tmptmp, 0, sizeof(tmptmp));
        //!
        for (size_t guessKey = 0; guessKey < 256; guessKey++) {
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
                        msbZeroAcc[point_cnt] += hammingWeigTable[trace_cnt][point_cnt];
                    }
                    msbZeroCnt++;
                }
                else if (((pt_tmp >> 7) & 0x01) == 1) {  // MSB == 1
                    for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
                        msbOneAcc[point_cnt] += hammingWeigTable[trace_cnt][point_cnt];
                    }
                    msbOneCnt++;
                }
            }
            
            //todo 포인트 별 평균 구하고 & 평균들의 차를 계산하고 (절댓값) & 최댓값 뽑기
            for (size_t point_cnt = 0; point_cnt < PointNum; point_cnt++) {
                msbZeroAvg[point_cnt] = (double)msbZeroAcc[point_cnt] / msbZeroCnt;
                msbOneAvg[point_cnt] = (double)msbOneAcc[point_cnt] / msbOneCnt;
                epsilon[point_cnt] = GetAbsoluteValue(msbOneAvg[point_cnt] - msbZeroAvg[point_cnt]);
            }
            
            // for (int k = 0; k < 64; k++) {
            //     printf("[%d]    %lf\n", k, epsilon[k]);
            // }
            // printf("\n");
            maxEpsilonCandid[guessKey][0] = GetMaxValue(epsilon, 64);     // 최댓값
            maxEpsilonCandid[guessKey][1] = GetSecondValue(epsilon, 64);     // 두번째로 큰 값
            // printf("%f\t %f\n", maxEpsilonCandid[guessKey][0], maxEpsilonCandid[guessKey][1]);
        }
        //todo 256개의 epsilon 최댓값으로 guesskey 확정하기
        for (int cnt_k = 0; cnt_k < 256; cnt_k++) {
            tmptmp[cnt_k] = maxEpsilonCandid[cnt_k][0];
            // for (int cnt_e = 0; cnt_e < 2; cnt_e++) {
            //     printf("[%02X]    %f\n",cnt_k, maxEpsilonCandid[cnt_k][cnt_e]);
            // }
            // printf("\n");
        }
        max_idx = GetMaxValueIndex(tmptmp, 256);
        double ttt = GetSecondValue(tmptmp, 256);
        finalGuessKey[guessByteIndex] = max_idx;
        // printf("%f\t %f", maxEpsilonCandid[max_idx][0], maxEpsilonCandid[max_idx][1]);
        finalRatio[guessByteIndex] = maxEpsilonCandid[max_idx][0] / ttt;
        
        printf("[%02d byte]\t %02X\t %lf\t %lf\n",
            guessByteIndex, finalGuessKey[guessByteIndex],
                maxEpsilonCandid[max_idx][0], finalRatio[guessByteIndex]);
    }

    

    fclose(fp_pt);
    fclose(fp_trace);
    fclose(fp_rst);

    return 0;
}
