#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MAX_THREADS 256
#define ROWS 20
#define COLS 20

struct argument
{
    int startRow;
    int nRows;
    long partialSum;
} threadArgs[MAX_THREADS];

long *bigMatrix;
#define MY_MQ_NAME "/my_mq"
static struct mq_attr my_mq_attr;
static mqd_t my_mq;
static int conter = 0;
static pthread_mutex_t mutexStatic;
static int sum = 0;
static int key = 0;
static long resultx;
static int nThreads;

static void *
thread_routine(void *arg)
{
    printf("thread routine %d \n", conter);

    int i, j;
    struct argument *currArg = (struct argument *)arg;
    printf("nrows %d\n", currArg->nRows);
    long sum = 0;
    for (i = 0; i < currArg->nRows; i++)
        for (j = 0; j < COLS; j++)
            sum += bigMatrix[(currArg->startRow + i) * COLS + j];
    currArg->partialSum = sum;
    printf("sum of process %d is %ld\n", conter, sum);
    key = 1;
    return NULL;
}

void *loop()
{

    int status;
    long recv_sum;
    int recv_counter;
    int counter_Locale = 0;
    while (1)
    {

        if (conter == nThreads)
        {
            //status = mq_receive(my_mq, (char *)&recv_sum,
            // sizeof(recv_sum), NULL);
            status = mq_receive(my_mq, (char *)&recv_sum,
                                sizeof(recv_sum), NULL);

            //printf("satut %d \n", status);
            if (status > 0)
            {
                counter_Locale = counter_Locale + 1;
                printf(":::::::::::::::::::::::::::::::::::::::::::::::::\n");
                printf("reciving message from process nbr %ld\n", recv_sum);
                sum = sum + recv_sum;
                printf("sum %ld\n", sum);
                status = 0;
                if (counter_Locale == nThreads)
                {
                    return NULL;
                }
            }
            else
            {
                // printf("waiting data \n");
            }
        }
        else
        {
            if (key == 1)
            {
                int resultx = threadArgs[conter].partialSum;
                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
                //imporatnt
                //you can't send a long varaibel
                //just int
                //why??????
                //i don't know
                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
                status = mq_send(my_mq, (const char *)&resultx, sizeof(resultx), 1);
                long result = threadArgs[conter].partialSum;
                //status = mq_send(my_mq, (const char *)&result, sizeof(result), 1);
                printf("sending data %d \n", resultx);
                key = 0;
                exit(0);
                return NULL;
            }
        }
    }
}
int main(int *argc, char *args[])
{
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    clock_t startTicks = clock();
    /* stuff to do! */
    printf("begun time %f", (double)startTicks);

    time_t start = time(0);
    /**args[0] = 'd';
    *args[1] = '5';
    *argc = 2;*/
    int nbr = 2;
    argc = &nbr;
    pthread_t threadLoop;
    pthread_t threadRoutine;
    printf("hello \n");

    long totalSum;
    int i, j, rowsPerThread, lastThreadRows;
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
    printf("Your matrix is as follows:\n");
    for (i = 0; i < ROWS; i++)
    {
        printf("[");
        for (j = 0; j < COLS; j++)
            printf("%ld:", bigMatrix[i * COLS + j]);
        printf("]\n");
    } //fin for

    //creation of precesses
    int iprocess = 0;

    // sscanf(args[1], "%d", &nThreads);
    while (iprocess < nThreads)
    {
        conter = iprocess;
        int pid = fork();
        // printf("pid\n");
        if (pid == 0)
        {
            printf("i am a child \n");

            break;
        }
        else
        {
            if (iprocess == nThreads - 1)
            {
                conter = nThreads;
            }
        }
        iprocess = iprocess + 1;
    }
    // wait(NULL);
    long size;
    my_mq_attr.mq_maxmsg = 10;
    my_mq_attr.mq_msgsize = sizeof(size);

    my_mq = mq_open(MY_MQ_NAME,
                    O_CREAT | O_RDWR | O_NONBLOCK,
                    0666,
                    &my_mq_attr);
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    printf("yes %d\n", conter);
    printf("nthreads is %d \n", nThreads);

    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    if (conter != nThreads)
    {
        printf("conter!!!%d\n", conter);
        printf("nrows %d \n", threadArgs[conter].nRows);
        pthread_create(&threadRoutine, NULL, thread_routine, &threadArgs[conter]);
    }
    else
    {
        printf("::::::::::::::::i am parent %d::::::::::: \n", conter);
    }

    pthread_create(&threadLoop, NULL, loop, NULL);

    //wait(NULL);

    if (conter != nThreads)
    {
        pthread_join(threadRoutine, NULL);
    }

    pthread_join(threadLoop, NULL);
    if (conter == nThreads)
    {
        printf("the finale Resutl is :: %ld\n", sum);
    }
    printf("finale %d \n", conter);

    printf("\nDuration of Processer usage: %f seconds\n", ((double)clock() - (double)startTicks) / CLOCKS_PER_SEC);

    gettimeofday(&tv2, NULL);
    printf("Total time = %f seconds\n",
           (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 +
               (double)(tv2.tv_sec - tv1.tv_sec));
    /* time(&start);
    printf("%s", ctime(&start));*/
    pthread_exit(NULL);

    return 0;
}