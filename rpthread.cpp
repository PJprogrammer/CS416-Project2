// File:	rpthread.c

// List all group member's name:
// username of iLab:
// iLab Server:

#include <cstdlib>
#include <iostream>
#include <vector>
#include "rpthread.h"

static void schedule();

using namespace std;

// INITAILIZE ALL YOUR VARIABLES HERE
// YOUR CODE HERE
int threadNum = 0; // Global var to assign thread ids
vector<tcb*> queue; // Scheduling queue
vector<tcb*> threadTable(100); // Threads Table
static ucontext_t mainThread;
uint currentThread = -1;

/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr,
                    void *(*function)(void*), void * arg) {
    *thread = threadNum;

    tcb* myTCB = new tcb();
    myTCB->id = threadNum;
    myTCB->status = READY;
    myTCB->stack = NULL;

    getcontext(&myTCB->context);

    myTCB->context.uc_link = NULL;
    myTCB->stack = malloc(1024*64);
    myTCB->context.uc_stack.ss_sp = myTCB->stack;
    myTCB->context.uc_stack.ss_size = 1024*64;
    myTCB->context.uc_stack.ss_flags = NULL;

    if(myTCB->stack == NULL) {
        cout << "Error: Unable to allocate stack memory: " << 1024*64 << " bytes in the heap\n";
    }

    makecontext(&myTCB->context, (void (*)()) &rpthread_start, 3, myTCB, function, arg);

    queue.push_back(myTCB);
    threadTable[threadNum] = myTCB;

    ++threadNum;
    schedule();


    // create Thread Control Block
    // create and initialize the context of this thread
    // allocate space of stack for this thread to run
    // after everything is all set, push this thread int
    // YOUR CODE HERE

    return 0;
};

void rpthread_start(tcb *myTCB, void (*function)(void *), void *arg) {
    myTCB->status = SCHEDULED;
    function(arg);
    myTCB->status = FINISHED;
}

/* give CPU possession to other user-level threads voluntarily */
int rpthread_yield() {

    // change thread state from Running to Ready
    // save context of this thread to its thread control block
    // wwitch from thread context to scheduler context

    // YOUR CODE HERE
    return 0;
};

/* terminate a thread */
void rpthread_exit(void *value_ptr) {
    tcb* myTCB = threadTable[currentThread];
    swapcontext(&myTCB->context, &mainThread);
    currentThread = -1;
    myTCB->status = FINISHED;
    myTCB->retVal = value_ptr;
    free(myTCB->stack);
    // Save Return value pointer in tcb


    // Deallocated any dynamic memory created when starting this thread

    // YOUR CODE HERE
};


/* Wait for thread termination */
int rpthread_join(rpthread_t thread, void **value_ptr) {

    // wait for a specific thread to terminate
    // de-allocate any dynamic memory created by the joining thread

    // YOUR CODE HERE
    return 0;
};

/* initialize the mutex lock */
int rpthread_mutex_init(rpthread_mutex_t *mutex,
                        const pthread_mutexattr_t *mutexattr) {
    //initialize data structures for this mutex

    // YOUR CODE HERE
    return 0;
};

/* aquire the mutex lock */
int rpthread_mutex_lock(rpthread_mutex_t *mutex) {
    // use the built-in test-and-set atomic function to test the mutex
    // if the mutex is acquired successfully, enter the critical section
    // if acquiring mutex fails, push current thread into block list and //
    // context switch to the scheduler thread

    // YOUR CODE HERE
    return 0;
};

/* release the mutex lock */
int rpthread_mutex_unlock(rpthread_mutex_t *mutex) {
    // Release mutex and make it available again.
    // Put threads in block list to run queue
    // so that they could compete for mutex later.

    // YOUR CODE HERE
    return 0;
};


/* destroy the mutex */
int rpthread_mutex_destroy(rpthread_mutex_t *mutex) {
    // Deallocate dynamic memory created in rpthread_mutex_init

    return 0;
};

/* scheduler */
static void schedule() {
    if(queue.empty()) {
        return;
    }

    tcb* myTCB = queue.back();
    currentThread = myTCB->id;
    swapcontext(&mainThread, &myTCB->context);

    // Every time when timer interrup happens, your thread library
    // should be contexted switched from thread context to this
    // schedule function

    // Invoke different actual scheduling algorithms
    // according to policy (RR or MLFQ)

    // if (sched == RR)
    //		sched_rr();
    // else if (sched == MLFQ)
    // 		sched_mlfq();

    // YOUR CODE HERE

// schedule policy
#ifndef MLFQ
    // Choose RR
#else
    // Choose MLFQ
#endif

}

/* Round Robin (RR) scheduling algorithm */
static void sched_rr() {
    // Your own implementation of RR
    // (feel free to modify arguments and return types)

    // YOUR CODE HERE
}

/* Preemptive MLFQ scheduling algorithm */
static void sched_mlfq() {
    // Your own implementation of MLFQ
    // (feel free to modify arguments and return types)

    // YOUR CODE HERE
}

// Feel free to add any other functions you need

// YOUR CODE HERE
