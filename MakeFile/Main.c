#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "thread_Operation.h"

int main(int argc, char *args[])
{

    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    clock_t startTicks = clock();
    printf("begun time %f", (double)startTicks);

    time_t start = time(0);
    printf("execute the main function");
    printf("My pid is %d", getpid());

    threadsOperation(&argc, args);

    printf("\nDuration of Processer usage: %f seconds\n", ((double)clock() - (double)startTicks) / CLOCKS_PER_SEC);
    gettimeofday(&tv2, NULL);
    printf("Total time = %f seconds\n",
           (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 +
               (double)(tv2.tv_sec - tv1.tv_sec));

    pthread_exit(NULL);
}