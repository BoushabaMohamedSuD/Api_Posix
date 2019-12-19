#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "ExternVariable.h"
#include "Calculate.h"
#include "DraweMatrix.h"

#define MAX_THREADS 256
#define ROWS 20
#define COLS 20
long *tempMatrix;
long *poidMatrix;
long *tempoidMatix;
struct argument
{
    int startRow;
    int nRows;
    long partialSum;
} threadArgs[MAX_THREADS];

long totalSum;
long wiehedSum;
pthread_mutex_t mutexStatic;
static void *thread_routine(void *arg)
{
    //printf("thread routine \n");
    struct argument *currArg = (struct argument *)arg;
    int nrows = currArg->nRows;
    int startRow = currArg->startRow;
    //printf("startrow %d\n", startRow);
    //printf("nrow %d \n", nrows);

    calculateSum(&nrows, tempMatrix, &startRow, &totalSum, &mutexStatic);
    calaulerProduit(&nrows, &mutexStatic,
                    &startRow, tempMatrix,
                    poidMatrix, tempoidMatix);
    weightedSum(&nrows, &mutexStatic,
                &startRow, tempMatrix,
                poidMatrix, &wiehedSum);

    return NULL;
}

void threadsOperation(int *argc, char *args[])
{
    int initMutex = pthread_mutex_init(&mutexStatic, NULL);
    totalSum = 0;
    wiehedSum = 0;
    pthread_t threads[MAX_THREADS];
    int i, j, nThreads;
    int rowsPerThread, lastThreadRows;
    printf("%d \n", *argc);

    printf("%s \n", args[0]);
    printf("%s \n", args[1]);
    if (*argc != 2)
    {
        printf("Usage: %s <numThreads>\n", args[0]);
        exit(0);
    }
    tempMatrix = malloc(ROWS * COLS * sizeof(long));
    poidMatrix = malloc(ROWS * COLS * sizeof(long));
    tempoidMatix = malloc(ROWS * COLS * sizeof(long));

    DrawMatrix(args, &nThreads, tempMatrix, &rowsPerThread, &lastThreadRows);
    printf("::::::::::poidMatrix::::::::::::::");
    DrawMatrix(args, &nThreads, poidMatrix, &rowsPerThread, &lastThreadRows);

    for (i = 0; i < nThreads; i++)
    {

        threadArgs[i].startRow = i * rowsPerThread;

        if (i == nThreads - 1)
            threadArgs[i].nRows = lastThreadRows;
        else
            threadArgs[i].nRows = rowsPerThread;
    }

    for (i = 0; i < nThreads; i++)
        pthread_create(&threads[i], NULL, thread_routine, &threadArgs[i]);

    for (i = 0; i < nThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\n::::::::: printing the results:::::::::::::\n");
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
            printf("M[%d,%d] =%ld \n", i, j, tempoidMatix[i * COLS + j]);
    }
    printf("\nThe total sum calculated by all the threads is %ld\n", totalSum);
    printf("\nThe Weithed sum calculated by all the threads is %ld\n", wiehedSum);
}
