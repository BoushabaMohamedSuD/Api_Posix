#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "ExternVariable.h"
#define MAX_THREADS 256
#define ROWS 20
#define COLS 20

void calculateSum(int *nrows, long *bigMatrix,
                  int *startrow, long *totalSum, pthread_mutex_t *mutex)
{

    //printf("calculate Sum \n");
    //printf("nrows %d\n", *nrows);
    //printf("startRows %d\n", *startrow);
    int i, j;
    for (i = 0; i < *nrows; i++)
    {
        // printf("ture\n");
        for (j = 0; j < COLS; j++)
        {
            //printf("false\n");
            pthread_mutex_lock(mutex);
            *totalSum = *totalSum + bigMatrix[(*startrow + i) * COLS + j];
            //*totalSum = *totalSum + 1;
            //printf("M  %ld \n", bigMatrix[9]);
            pthread_mutex_unlock(mutex);
        }
    }
    // printf("end \n");
}
void calaulerProduit(int *nrows, pthread_mutex_t *mutex,
                     int *startrow, long *tempMatrix,
                     long *poidMatrix, long *tempoidMatrix)
{

    //printf("calculate Sum \n");
    //printf("nrows %d\n", *nrows);
    //printf("startRows %d\n", *startrow);
    int i, j, i1, j1;

    for (i = 0; i < *nrows; i++)
    {
        // printf("ture\n");
        for (j = 0; j < COLS; j++)
        {
            //printf("false\n");
            long result = 0;
            for (i1 = 0; i1 < ROWS; i1++)
            {
                result = result + tempMatrix[(*startrow + i) * COLS + i1] * poidMatrix[(*startrow + i1) * COLS + j];
            }

            pthread_mutex_lock(mutex);

            tempoidMatrix[(*startrow + i) * COLS + j] = result;
            //printf("result : %ld \n", result);
            //printf("M[%d,%d]=%ld \n", (*startrow + i), j, tempoidMatrix[(*startrow + i) * COLS + j]);
            //*totalSum = *totalSum + 1;
            //printf("M  %ld \n", bigMatrix[9]);
            pthread_mutex_unlock(mutex);
        }
    }

    // printf("end \n");
}
void weightedSum(int *nrows, pthread_mutex_t *mutex,
                 int *startrow, long *tempMatrix,
                 long *poidMatrix, long *WeithedSum)
{
    int i, j, i1, j1;

    for (i = 0; i < *nrows; i++)
    {
        // printf("ture\n");
        for (j = 0; j < COLS; j++)
        {

            pthread_mutex_lock(mutex);
            *WeithedSum = *WeithedSum + tempMatrix[(*startrow + i) * COLS + j] * poidMatrix[(*startrow + i) * COLS + j];
            pthread_mutex_unlock(mutex);
        }
    }
}