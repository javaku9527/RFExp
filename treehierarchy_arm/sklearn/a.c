#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

const int32_t NUM_RUNS = 500;

float** readFloatsFromFile(const char* filepath, int* numRows, int* numCols);
float predict(float* test, float* result);

int main(int argc, char *argv[]) {
    int numRows, numCols;

    char filepath[1024];
    sprintf(filepath, "/home/chku/treehierarchy/data/sklearn_models/%s.test.csv", argv[1]);
    float** data = readFloatsFromFile(filepath, &numRows, &numCols);

    struct timeval start, end;
    double dif = 0;

    gettimeofday(&start, NULL);
    for (int32_t trial = 0; trial < NUM_RUNS; trial++)
    {
      for (int32_t i = 0; i < numRows; i++)
      {
        float *input = data[i] + 1;
        float result[] = {0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0};
        predict(input, result);
        // printf("%.2f %.2f\t", result[0], result[1]);
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