#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

struct Memref {
  float *bufferPtr;
  float *alignedPtr;
  int64_t offset;
  int64_t lengths[1];
  int64_t strides[1];
};

const int32_t NUM_RUNS = 500;

float** readFloatsFromFile(const char* filepath, int* numRows, int* numCols);
int32_t Init_model();
struct Memref Prediction_Function(float* ptr, float* alignedPtr, int64_t offset, int64_t m_batchSize, int64_t rowSize, int64_t rowSize2, int64_t stride, float* resultPtr, float* resultAlignedPtr, int64_t offset2, int64_t resultLen, int64_t stride2);

int main(int argc, char *argv[]) {
    int numRows, numCols;
    
    char filepath[1024];
    sprintf(filepath, "/home/chku/treebeard/llvm-project/mlir/examples/treebeard/xgb_models/%s_xgb_model_save.json.test.sampled.csv", argv[1]);
    float** data = readFloatsFromFile(filepath, &numRows, &numCols);
    
    if(numRows != 2000) {
        printf("Wrong Input %d\n", numRows);
        return -1;
    }

    struct timeval start, end;
    double dif;
    Init_model();
    gettimeofday(&start, NULL);
    for (int32_t trial = 0; trial < NUM_RUNS; trial++) {
      for (int32_t i = 0; i < numRows; i++) {
        float *input = data[i];
        float returnValue[] = {-1, -1, -1, -1, -1 ,-1, -1, -1, -1, -1, -1 ,-1, -1, -1, -1, -1, -1 ,-1, -1, -1, -1, -1, -1 ,-1, -1, -1};
        int32_t m_rowSize = numCols - 1;
        int32_t m_batchSize = 1;

        float *ptr = input, *alignedPtr = input;
        int64_t rowSize = m_rowSize, offset = 0, stride = 1;
        float *resultPtr = returnValue, *resultAlignedPtr = returnValue;
        int64_t resultLen = m_batchSize;
        Prediction_Function(ptr, alignedPtr, offset, m_batchSize, rowSize, rowSize, stride, resultPtr, resultAlignedPtr, offset, resultLen, stride);
       }
    }
    gettimeofday(&end, NULL);

    dif = (end.tv_sec - start.tv_sec) * 1000.0;
    dif += (end.tv_usec - start.tv_usec) / 1000.0;

    printf("%.2f+", dif);
    // printf("\n共跑了%d個Test Case, 花費時間：%.2f ms\n", numRows * NUM_RUNS, dif);
}

float** readFloatsFromFile(const char* filepath, int* numRows, int* numCols) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    *numRows = 0;
    *numCols = 0;
    char line[60000];
    while (fgets(line, sizeof(line), file)) {
        (*numRows)++;
        char* token = strtok(line, ",");
        int count = 0;
        while (token != NULL) {
            count++;
            token = strtok(NULL, ",");
        }
        if (count > *numCols) {
            *numCols = count;
        }
    }

    fseek(file, 0, SEEK_SET);

    float** data = (float**)malloc((*numRows) * sizeof(float*));
    for (int i = 0; i < *numRows; ++i) {
        data[i] = (float*)malloc((*numCols) * sizeof(float));
    }

    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        int col = 0;
        while (token != NULL) {
            data[row][col] = atof(token);
            col++;
            token = strtok(NULL, ",");
        }
        row++;
    }

    fclose(file);
    return data;
}