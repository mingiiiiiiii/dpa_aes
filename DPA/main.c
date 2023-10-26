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

struct Element {
    double value;
    uint32_t originalIndex;
};

void merge(struct Element arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    struct Element L[n1], R[n2];

    // arr값 대입
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int i = 0; i < n2; i++) {
        R[i] = arr[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].value <= R[j].value) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Merge Sort 함수
void mergeSort(struct Element arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}


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

    uint8_t pt_tmp = 0x00;    // 평문 특정 바이트 값을 위한 임시변수
    uint32_t msbZeroCnt, msbOneCnt;
    uint32_t msbZeroAcc[PointNum], msbOneAcc[PointNum];     // 해밍웨이트 누적 합을 저장하는 배열 (MSB에 따른 분류)
    double msbZeroAvg[PointNum], msbOneAvg[PointNum], epsilon[PointNum];    // 포인트들의 해밍웨이트 평균 값 (MSB에 따른 분류), 평균들의 차를 저장하는 배열
    double maxEpsilonCandid[256];   // guessKey 00~ff에서 각각의 epsilon에서의 최댓값을 저장하는 배열
    uint8_t finalGuessKey[16] = { 0x00 };
    //! finalRatio는 테이블 필요 없을듯
    double finalRatio[16] = { 0x00 };
    int max_idx;
    // double tmptmp[256] = { 0x00 };

    //!test********************************
    struct Element arr[256] = { {0,0} };
    int n_size = 256;
    //!************************************

    for (size_t guessByteIndex = 0; guessByteIndex < 16; guessByteIndex++) {
        // initialize
        memset(maxEpsilonCandid, 0, sizeof(maxEpsilonCandid));
        max_idx = 0;

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
            //! test
            arr[guessKey].originalIndex = guessKey;
            arr[guessKey].value = GetMaxValue(epsilon, 64);
            //!

            maxEpsilonCandid[guessKey] = GetMaxValue(epsilon, 64);     // 최댓값
        }

        //! test
        mergeSort(arr, 0, 256 - 1);
        printf("정렬된 배열:\n");
        for (int i = 0; i < 256; i++) {
        printf("값: %lf, 원래 인덱스: %d\n", arr[i].value, arr[i].originalIndex);
        }

        //todo 256개의 epsilon 최댓값으로 guesskey 확정하기
        max_idx = GetMaxValueIndex(maxEpsilonCandid, 256);
        double ttt = GetSecondValue(maxEpsilonCandid, 256);
        finalGuessKey[guessByteIndex] = max_idx;
        finalRatio[guessByteIndex] = maxEpsilonCandid[max_idx] / ttt;
        
        printf("[%02d byte]\t %02X\t %lf\t %lf\n",
            guessByteIndex, finalGuessKey[guessByteIndex],
            maxEpsilonCandid[max_idx], finalRatio[guessByteIndex]);
        fprintf(fp_rst, "[%02d byte]\t %02X\t %lf\t %lf\n",
            guessByteIndex, finalGuessKey[guessByteIndex],
                maxEpsilonCandid[max_idx], finalRatio[guessByteIndex]);
    }

    fclose(fp_pt);
    fclose(fp_trace);
    fclose(fp_rst);

    return 0;
}
