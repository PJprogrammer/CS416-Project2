#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include "../rpthread.h"


void* myprint(void *x)
{
    int k = *((int *)x);
    printf("Helper Thread: Thread created.. value of k [%d]\n",k);

    int* ret = (int*) malloc(sizeof(int ));
    *ret = k + 10;
    printf("Helper Thread: New value of k [%d]\n", *ret);

    pthread_exit(ret);
}


int main()
{
    pthread_t th1;
    int x = 5;
    void *y;

    pthread_create(&th1, NULL, myprint, (void*)&x);
    pthread_join(th1, &y);

    printf("Main Thread: Exit value is [%d]\n",*((int*)y));
}
