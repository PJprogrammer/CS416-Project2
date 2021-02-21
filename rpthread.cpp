// File:	rpthread.c

// List all group member's name: Marko Elez (me470), Paul John (pj242)
// username of iLab:
// iLab Server: cd.cs.rutgers.edu

#include <cstdlib>
#include <iostream>
#include <vector>
#include <atomic>
#include "rpthread.h"

#define SCHEDULER_THREAD 0
#define MAIN_THREAD 1

int threadNum = 2; // Global var to assign thread ids
std::vector<tcb*> queue; // Scheduling queue
std::vector<tcb*> threadTable(100); // Threads Table

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
        std::cout << "Error: Unable to allocate stack memory: " << 1024*64 << " bytes in the heap\n";
    }

    makecontext(&newThread->context, (void (*)()) &rpthread_start, 3, newThread, function, arg);
    threadTable[threadNum] = newThread;

    ++threadNum; // Increment thread id counter

    // Initialize the scheduler & main thread
    if(!isSchedCreated) {
        createSchedulerContext();
        createMainContext();

        queue.push_back(newThread);
        queue.push_back(threadTable[MAIN_THREAD]); // mainTCB

        isSchedCreated = true;

        // Swap from main thread to scheduler thread
        currentThread = 0;
        swapcontext(&threadTable[MAIN_THREAD]->context, &threadTable[SCHEDULER_THREAD]->context);
    } else {
        queue.insert(queue.end()-1, newThread);
    }

    return 0;
};

void rpthread_start(tcb *currTCB, void (*function)(void *), void *arg) {
    currTCB->status = SCHEDULED;
    function(arg);
    currTCB->status = FINISHED;
    free(currTCB->context.uc_stack.ss_sp);

    currentThread = 0;
    swapcontext(&currTCB->context, &threadTable[SCHEDULER_THREAD]->context);
}

/* give CPU possession to other user-level threads voluntarily */
int rpthread_yield() {
    tcb* currTCB = threadTable[currentThread];
    currTCB->status = READY;

    currentThread = 0;
    swapcontext(&currTCB->context, &threadTable[SCHEDULER_THREAD]->context);

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
    setcontext(&threadTable[SCHEDULER_THREAD]->context);
};


/* Wait for thread termination */
int rpthread_join(rpthread_t thread, void **value_ptr) {
    tcb* currTCB = threadTable[currentThread];
    tcb* joinedTCB = threadTable[thread];

    if(joinedTCB->status != FINISHED) {
        joinedTCB->joiningThread = currTCB->id;
        currTCB->status = BLOCKED;

        currentThread = 0;
        swapcontext(&currTCB->context, &threadTable[SCHEDULER_THREAD]->context);
    }

    value_ptr = &joinedTCB->retVal;

    return 0;
};

/* initialize the mutex lock */
int rpthread_mutex_init(rpthread_mutex_t *mutex,
                        const pthread_mutexattr_t *mutexattr) {

    return 0;
};

/* aquire the mutex lock */
int rpthread_mutex_lock(rpthread_mutex_t *mutex) {
    bool prev = std::atomic_flag_test_and_set_explicit(&mutex->flag, std::memory_order_acquire);
    tcb* currTCB = threadTable[currentThread];

    if(prev) { // flag was previously true, so mutex is not acquired successfully
        mutex->queue.push_back(currTCB->id);
        currTCB->status = BLOCKED;

        currentThread = 0;
        swapcontext(&currTCB->context, &threadTable[SCHEDULER_THREAD]->context);
    }

    return 0;
};

/* release the mutex lock */
int rpthread_mutex_unlock(rpthread_mutex_t *mutex) {
    std::atomic_flag_clear_explicit(&mutex->flag, std::memory_order_release);

    for(uint i : mutex->queue) {
        queue.insert(queue.begin(), threadTable[i]);
    }

    return 0;
};


/* destroy the mutex */
int rpthread_mutex_destroy(rpthread_mutex_t *mutex) {

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

void createSchedulerContext() {
    tcb* schedTCB = new tcb();
    schedTCB->id = 0;
    getcontext(&schedTCB->context);
    schedTCB->context.uc_link = NULL;
    schedTCB->context.uc_stack.ss_sp = malloc(STACK_SIZE);
    schedTCB->context.uc_stack.ss_size = STACK_SIZE;
    schedTCB->context.uc_stack.ss_flags = NULL;

    if(schedTCB->context.uc_stack.ss_sp == NULL) {
        std::cout << "Error: Unable to allocate stack memory: " << STACK_SIZE << " bytes in the heap\n";
    }

    makecontext(&schedTCB->context, (void (*)()) &schedule, 0);

    threadTable[SCHEDULER_THREAD] = schedTCB;
}

void createMainContext() {
    tcb* mainTCB = new tcb();
    mainTCB->id = 1;
    mainTCB->status = READY;

    threadTable[MAIN_THREAD] = mainTCB;
}
