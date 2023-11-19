#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TraceFileName "powerConsumption.trace"
#define ResultFileName "realPowerConsumption.txt"

int main() {
    FILE* fp_trace;     // for trace file
    FILE* fp_rst;       // for result file

    fp_trace = fopen(TraceFileName, "rb");
    if (fp_trace == NULL) {
        fprintf(stderr, "File Open Error\n");
    }

    fp_rst = fopen(ResultFileName, "w");
        if (fp_rst == NULL) {
        fprintf(stderr, "File Open Error\n");
    }

// file introduce
char* buf = (char*)malloc(sizeof(char) * 20);
fread(buf, sizeof(char), 20, fp_trace);
printf("%s\n", buf);

// read trace number
int traceNum = 0;
fread(&traceNum, sizeof(int), 1, fp_trace);
printf("traceNum = %d\n", traceNum);    // 5000

// read point number
int pointNum = 0;
fread(&pointNum, sizeof(int), 1, fp_trace);
printf("pointNum = %d\n", pointNum);    // 4984

// "END!"
char* buf2 = (char*)malloc(sizeof(char) * 4);
fread(buf2, sizeof(char), 4, fp_trace);
printf("%s\n", buf2);

    float** buf3 = (float**)malloc(sizeof(float*) * traceNum);
    for (size_t cnt_i = 0; cnt_i < traceNum; cnt_i++) {
        buf3[cnt_i] = (float*)malloc(sizeof(float) * pointNum);
        fread(buf3[cnt_i], sizeof(float), pointNum, fp_trace);
        // printf("buf[%d] = ", cnt_i);
        for (size_t cnt_j = 0; cnt_j < pointNum; cnt_j++) {
            fprintf(fp_rst, "%f ", buf3[cnt_i][cnt_j]);
        }
        fprintf(fp_rst, "\n");
    }

    for (size_t cnt_i = 0; cnt_i < traceNum; cnt_i++) {
        free(buf3[cnt_i]);
    }
    free(buf3);

    fclose(fp_trace);
    fclose(fp_rst);

    return 0;
}

