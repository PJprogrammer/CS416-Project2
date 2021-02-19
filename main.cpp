#include <iostream>
#include <vector>
#include "rpthread.h"

using namespace std;

void* test(void*) {

  cout << "THIS IS A SEPARATE THREAD\n";

  rpthread_exit(NULL);

}

struct hello {
    int myID;
    string myName;
};

int main() {
    cout << "Hello World!\n";

    rpthread_t t1, t2, t3;

    rpthread_create(&t1, NULL, test, NULL);
    rpthread_create(&t2, NULL, test, NULL);
    rpthread_create(&t3, NULL, test, NULL);
    

    return 0;
}
