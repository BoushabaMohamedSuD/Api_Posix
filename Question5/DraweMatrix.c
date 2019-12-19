#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#define MAX_THREADS 256
#define ROWS 20
#define COLS 20

void DrawMatrix(char *args[], int *nThreads, long *bigMatrix, int *rowsPerThread, int *lastThreadRows)
{

    int i, j;
    printf("let's go\n");
    sscanf(args[1], "%d", nThreads);
    printf("nbr of threads : %d \n", *nThreads);

        for (i = 0; i < ROWS; i++)
        for (j = 0; j < COLS; j++)
            bigMatrix[i * COLS + j] = i + COLS * j;

    *rowsPerThread = ROWS / *nThreads;
    if (ROWS % *nThreads == 0)
        *lastThreadRows = *rowsPerThread;
    else
        *lastThreadRows = *rowsPerThread + ROWS % *nThreads;

    printf("Your matrix is as follows:\n");
    for (i = 0; i < ROWS; i++)
    {
        printf("[");
        for (j = 0; j < COLS; j++)
            printf("%ld:", bigMatrix[i * COLS + j]);
        printf("]\n");
    }
}