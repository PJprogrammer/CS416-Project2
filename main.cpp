#include <iostream>
#include <csignal>
#include "rpthread.h"

using namespace std;

rpthread_mutex_t  mutex;

void* test1(void*) {
    cout << "Thread 1: Before yield\n";

    rpthread_mutex_lock(&mutex);
    rpthread_yield();
    rpthread_mutex_unlock(&mutex);

    cout << "Thread 1: Hello after yield\n";

    rpthread_exit(NULL);
}

void* test2(void*) {
    cout << "Thread 2: Entered after yield from Thread 1\n";

    rpthread_mutex_lock(&mutex);
    cout << "Thread 2: Hello \n";
    rpthread_mutex_unlock(&mutex);

    rpthread_exit(NULL);
}


int main() {
    cout << "Hello World!\n";

    rpthread_mutex_init(&mutex, NULL);

    rpthread_t t1, t2, t3, t4, t5, t6;

    rpthread_create(&t1, NULL, test1, NULL);
    rpthread_create(&t2, NULL, test2, NULL);
    rpthread_create(&t3, NULL, test2, NULL);
    rpthread_create(&t4, NULL, test2, NULL);
    rpthread_create(&t5, NULL, test2, NULL);
    rpthread_create(&t6, NULL, test2, NULL);


    rpthread_join(t1, NULL);
    rpthread_join(t2, NULL);
    rpthread_join(t3, NULL);
    rpthread_join(t4, NULL);
    rpthread_join(t5, NULL);
    rpthread_join(t6, NULL);

    return 0;
}
