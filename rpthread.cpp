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
int threadNum = 2; // Global var to assign thread ids
vector<tcb*> queue; // Scheduling queue
vector<tcb*> threadTable(100); // Threads Table

uint currentThread = 1;
bool isSchedCreated = false;

/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr,
                    void *(*function)(void*), void * arg) {

    *thread = threadNum;

    // Initialize new thread
    tcb* newThread = new tcb();
    newThread->id = threadNum;
    newThread->status = READY;
    newThread->joiningThread = 0;
    getcontext(&newThread->context);
    newThread->context.uc_stack.ss_sp = malloc(STACK_SIZE);
    newThread->context.uc_stack.ss_size = STACK_SIZE;
    newThread->context.uc_stack.ss_flags = NULL;

    if(newThread->context.uc_stack.ss_sp == NULL) {
        cout << "Error: Unable to allocate stack memory: " << 1024*64 << " bytes in the heap\n";
    }

    makecontext(&newThread->context, (void (*)()) &rpthread_start, 3, newThread, function, arg);
    threadTable[threadNum] = newThread;

    ++threadNum; // Increment thread id counter

    // Initialize the scheduler & main thread
    if(!isSchedCreated) {
        tcb* schedTCB = new tcb();
        schedTCB->id = 0;
        getcontext(&schedTCB->context);
        schedTCB->context.uc_link = NULL;
        schedTCB->context.uc_stack.ss_sp = malloc(STACK_SIZE);
        schedTCB->context.uc_stack.ss_size = STACK_SIZE;
        schedTCB->context.uc_stack.ss_flags = NULL;

        if(schedTCB->context.uc_stack.ss_sp == NULL) {
            cout << "Error: Unable to allocate stack memory: " << STACK_SIZE << " bytes in the heap\n";
        }

        makecontext(&schedTCB->context, (void (*)()) &schedule, 0);
        threadTable[0] = schedTCB;

        tcb* mainTCB = new tcb();
        mainTCB->id = 1;
        mainTCB->status = READY;
        threadTable[1] = mainTCB;

        queue.push_back(newThread);
        queue.push_back(threadTable[1]); // mainTCB

        isSchedCreated = true;

        // Swap from main thread to scheduler thread
        currentThread = 0;
        swapcontext(&threadTable[1]->context, &threadTable[0]->context);
    } else {
        queue.insert(queue.end()-1, newThread);
    }

    // create Thread Control Block
    // create and initialize the context of this thread
    // allocate space of stack for this thread to run
    // after everything is all set, push this thread int
    // YOUR CODE HERE

    return 0;
};

void rpthread_start(tcb *currTCB, void (*function)(void *), void *arg) {
    currTCB->status = SCHEDULED;
    function(arg);
    currTCB->status = FINISHED;
    free(currTCB->context.uc_stack.ss_sp);

    currentThread = 0;
    swapcontext(&currTCB->context, &threadTable[0]->context);
}

/* give CPU possession to other user-level threads voluntarily */
int rpthread_yield() {
    tcb* currTCB = threadTable[currentThread];
    currTCB->status = READY;

    currentThread = 0;
    swapcontext(&currTCB->context, &threadTable[0]->context);

    // change thread state from Running to Ready
    // save context of this thread to its thread control block
    // wwitch from thread context to scheduler context

    // YOUR CODE HERE
    return 0;
};

/* terminate a thread */
void rpthread_exit(void *value_ptr) {
    tcb* currTCB = threadTable[currentThread];
    currTCB->status = FINISHED;
    currTCB->retVal = value_ptr;
    free(currTCB->context.uc_stack.ss_sp);

    if(currTCB->joiningThread != 0) {
        tcb* joinedTCB = threadTable[currTCB->joiningThread];
        joinedTCB->status = READY;
        queue.insert(queue.begin(), joinedTCB);
    }

    currentThread = 0;
    setcontext(&threadTable[0]->context);

    // Deallocated any dynamic memory created when starting this thread

    // YOUR CODE HERE
};


/* Wait for thread termination */
int rpthread_join(rpthread_t thread, void **value_ptr) {
    tcb* currTCB = threadTable[currentThread];
    tcb* joinedTCB = threadTable[thread];

    if(joinedTCB->status != FINISHED) {
        joinedTCB->joiningThread = currTCB->id;
        currTCB->status = BLOCKED;

        currentThread = 0;
        swapcontext(&currTCB->context, &threadTable[0]->context);
    }

    value_ptr = &joinedTCB->retVal;

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

    if(queue.back()->status == FINISHED || queue.back()->status == BLOCKED) {
        queue.pop_back();
    } else {
        queue.insert(queue.begin(), queue.back());
        queue.pop_back();
    }

    tcb* currTCB = queue.back();
    currentThread = currTCB->id;
    setcontext(&currTCB->context);

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

