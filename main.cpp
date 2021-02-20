#include <iostream>
#include <csignal>
#include "rpthread.h"

using namespace std;

void* test1(void*) {
  cout << "Thread 1: Entered before yield\n";

  rpthread_yield();

  cout << "Thread 1: Hello after yield\n";

  rpthread_exit(NULL);

}

void* test2(void*) {
    cout << "Thread 2: Entered before yield\n";

    rpthread_yield();


    cout << "Thread 2: Hello after yield\n";

    rpthread_exit(NULL);

}


int main() {
    cout << "Hello World!\n";

    rpthread_t t1, t2, t3;

    rpthread_create(&t1, NULL, test1, NULL);
    rpthread_create(&t2, NULL, test2, NULL);

    rpthread_join(t1, NULL);
    rpthread_join(t2, NULL);

    return 0;
}
