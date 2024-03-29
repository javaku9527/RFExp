#include "header.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

const int32_t NUM_RUNS = 500;

double **readFromFile(const char *filepath, int *numRows, int *numCols);

int main(int argc, char *argv[])
{
    int numRows, numCols;

    char filepath[1024];
    sprintf(filepath, "/home/chku/treehierarchy/data/sklearn_models/%s.test.csv", argv[1]);
    double **datas = readFromFile(filepath, &numRows, &numCols);

    struct timeval start, end;
    double dif = 0;

    gettimeofday(&start, NULL);
    for (int32_t trial = 0; trial < NUM_RUNS; trial++)
    {
        for (int32_t i = 0; i < numRows; i++)
        {
            double result[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            double *data = datas[i] + 1;

            union Entry *input = (union Entry *)malloc((numCols - 1) * sizeof(union Entry));
            
            for (int x = 0; x < numCols - 1; x++)
                input[x].qvalue = quantize(data[x], x);
            predict(input, 1, result);
        }
    }
    gettimeofday(&end, NULL);

    dif = (end.tv_sec - start.tv_sec) * 1000.0;
    dif += (end.tv_usec - start.tv_usec) / 1000.0;

    printf("%.2f+", dif);
}

double **readFromFile(const char *filepath, int *numRows, int *numCols)
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    *numRows = 0;
    *numCols = 0;
    char line[60000];
    while (fgets(line, sizeof(line), file))
    {
        (*numRows)++;
        char *token = strtok(line, ",");
        int count = 0;
        while (token != NULL)
        {
            count++;
            token = strtok(NULL, ",");
        }
        if (count > *numCols)
        {
            *numCols = count;
        }
    }

    fseek(file, 0, SEEK_SET);

    double **data = (double **)malloc((*numRows) * sizeof(double *));
    for (int i = 0; i < *numRows; ++i)
    {
        data[i] = (double *)malloc((*numCols) * sizeof(double));
    }

    int row = 0;
    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        int col = 0;
        while (token != NULL)
        {
            data[row][col] = atof(token);
            col++;
            token = strtok(NULL, ",");
        }
        row++;
    }

    fclose(file);
    return data;
}