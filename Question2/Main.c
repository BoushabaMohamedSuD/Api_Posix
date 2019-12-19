#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS 256
#define ROWS 20 /*10000*/
#define COLS 20 /*10000*/

/* Arguments exchanged with threads */

struct argument
{
    int startRow;
    int nRows;
    long partialSum;
} threadArgs[MAX_THREADS];

/* Matrix pointer: it will be dynamically allocated */
long *bigMatrix;
/* Thread routine : make the summation of all the elements of the assigned matrix rows */
static void *thread_routine(void *arg)
{
    printf("thread routine \n");
    int i, j;
    /* Type−cast passed pointer to expected structure containing the start row,the number of rows to be summed and the return sum argument */
    struct argument *currArg = (struct argument *)arg;
    long sum = 0;
    for (i = 0; i < currArg->nRows; i++)
        for (j = 0; j < COLS; j++)
            sum += bigMatrix[(currArg->startRow + i) * COLS + j];
    currArg->partialSum = sum;
    return NULL;
} //fin thread_routine()
void calculateSum()
{
    printf("calculate Sum \n");
}
void calculateMoyenne()
{
    printf("calaculate average \n");
}
void calaculateMedaine()
{
    printf("calaculate Medaine \n");
}

void threadsOperation(int *argc, char *args[])
{
    pthread_t threads[MAX_THREADS];
    long totalSum;
    int i, j, nThreads, rowsPerThread, lastThreadRows;
    /* Get the number of threads from command parameters */
    printf("%d \n", *argc);
    /*args[0] = "welcome";
    args[1] = "hello";*/
    printf("%s \n", args[0]);
    printf("%s \n", args[1]);
    if (*argc != 2)
    {
        printf("Usage: %s <numThreads>\n", args[0]);
        exit(0);
    } //fin if
    printf("let's go\n");
    sscanf(args[1], "%d", &nThreads);
    printf("nbr of threads : %d \n", nThreads);
    /* Allocate the matrix M */
    bigMatrix = malloc(ROWS * COLS * sizeof(long));
    /* Fill the matrix with some values */
    printf("nice1 \n");
    for (i = 0; i < ROWS; i++)
        for (j = 0; j < COLS; j++)
            bigMatrix[i * COLS + j] = i + COLS * j;

    /* If the number of rows cannot be divided exactly by the number of threads,let the last thread handle also the remaining rows */
    rowsPerThread = ROWS / nThreads;
    if (ROWS % nThreads == 0)
        lastThreadRows = rowsPerThread;
    else
        lastThreadRows = rowsPerThread + ROWS % nThreads;
    /* Prepare arguments for threads */
    printf("nice2 \n");
    for (i = 0; i < nThreads; i++)
    {
        /* Prepare Thread arguments */

        threadArgs[i].startRow = i * rowsPerThread;

        if (i == nThreads - 1)
            threadArgs[i].nRows = lastThreadRows;
        else
            threadArgs[i].nRows = rowsPerThread;
    } //fin for
    /* Start the threads using default thread attributes */
    printf("nice3 \n");
    for (i = 0; i < nThreads; i++)
        pthread_create(&threads[i], NULL, thread_routine, &threadArgs[i]);
    /* Display the values of the matrix */
    printf("Your matrix is as follows:\n");
    for (i = 0; i < ROWS; i++)
    {
        printf("[");
        for (j = 0; j < COLS; j++)
            printf("%ld:", bigMatrix[i * COLS + j]);
        printf("]\n");
    } //fin for
    printf("Please hit enter to continue ...");
    // getchar();
    /* Wait thread termination and use the corresponding sum value for the final summation */
    totalSum = 0;
    for (i = 0; i < nThreads; i++)
    {
        pthread_join(threads[i], NULL);
        printf("Thread %d just terminated with its partial sum %ld\n", i, threadArgs[i].partialSum);
        totalSum += threadArgs[i].partialSum;
    } //fin for

    /* Display the total sum calculated */
    printf("The total sum calculated by all the threads is %ld\n", totalSum);
}
void processOperation(int *argc, char *args[])
{
}

int main(int argc, char *args[])
{
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    clock_t startTicks = clock();
    /* stuff to do! */
    printf("begun time %f", (double)startTicks);

    time_t start = time(0);
    printf("execute the main function");
    printf("My pid is %d", getpid());

    threadsOperation(&argc, args);

    /* Array of thread identifiers */

    //getchar();
    // printf("time of execution %f\n", difftime(end, start));
    //printf("time of execution is %lf \n", clock() / CLOCKS_PER_SEC);

    /*int v = 1;
    while (v < 1000000)
    {

        v = v + 1;
    }*/

    // printf("\nDuration: %f seconds\n", (double)startTicks);
    //time_t end = time(0);
    //sleep(2);
    // printf("\ntime of exection the application %f \n", end);

    printf("\nDuration of Processer usage: %f seconds\n", ((double)clock() - (double)startTicks) / CLOCKS_PER_SEC);

    gettimeofday(&tv2, NULL);
    printf("Total time = %f seconds\n",
           (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 +
               (double)(tv2.tv_sec - tv1.tv_sec));
    /* time(&start);
    printf("%s", ctime(&start));*/
    pthread_exit(NULL);

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
    //clock calcule the time of cpu execution
    //not real time of the applicatio

} //fin main