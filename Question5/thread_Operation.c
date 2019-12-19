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
long *bigMatrix;

struct argument
{
    int startRow;
    int nRows;
    long partialSum;
} threadArgs[MAX_THREADS];
long totalSum;
pthread_mutex_t mutexStatic;
static void *thread_routine(void *arg)
{
    printf("thread routine \n");
    struct argument *currArg = (struct argument *)arg;
    int nrows = currArg->nRows;
    int startRow = currArg->startRow;
    printf("startrow %d\n", startRow);
    printf("nrow %d \n", nrows);
    calculateSum(&nrows, bigMatrix, &startRow, &totalSum, &mutexStatic);

    return NULL;
}

void threadsOperation(int *argc, char *args[])
{
    int initMutex = pthread_mutex_init(&mutexStatic, NULL);
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
    bigMatrix = malloc(ROWS * COLS * sizeof(long));

    DrawMatrix(args, &nThreads, bigMatrix, &rowsPerThread, &lastThreadRows);

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

    printf("The total sum calculated by all the threads is %ld\n", totalSum);
}
