#include <iostream>
#include "rpthread.h"

using namespace std;

void* test() {

  cout << "THIS IS A SEPARATE THREAD";

}

int main() {

    rpthread_t t1;

    rpthread_create(t1, null, test, null);
    
    cout << "Hello World!";
    return 0;
}
