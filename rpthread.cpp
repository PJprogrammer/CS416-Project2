// File:	rpthread.c

// List all group member's name: Marko Elez (me470), Paul John (pj242)
// username of iLab:
// iLab Server: cd.cs.rutgers.edu

#include <cstdlib>
#include <iostream>
#include <vector>
#include <atomic>
#include <csignal>
#include <sys/time.h>
#include <cstring>
#include "rpthread.h"
#include "hashmap.h"

#define SCHEDULER_THREAD 0
#define MAIN_THREAD 1
#define QUEUE_NUM 4

enum sched_options {RR, MLFQ};
int SCHED_TYPE = MLFQ;

int threadCounter = 2; // Global var to assign thread ids

// K: thread ID, V: tcb*
HashMap<int, tcb*> *map = new HashMap<int, tcb*>;

Queue<tcb*> run_queue[QUEUE_NUM];

tTuple currentThread = {MAIN_THREAD, 0};
bool isSchedCreated = false;
bool isYielding = false;
bool isCurrThreadRemoved = false;

void setupTimer();
void timer_handler(int signum);
void createSchedulerContext();
void createMainContext();

static void schedule();
static int sched_rr(int queueNum);
static void sched_mlfq();

bool isLastQueue(int queueNum);
bool isThreadInactive(int queueNum);

tcb* get_current_tcb() {
  return map->get(currentThread.tNum);
}

tcb* get_scheduler_tcb() {
  return map->get(SCHEDULER_THREAD);
}

/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg) {
    *thread = threadCounter;

    // Initialize new thread
    tcb* newThread = new tcb();
    newThread->id = threadCounter;
    newThread->status = READY;
    newThread->joiningThread = 0;
    getcontext(&newThread->context);
    newThread->context.uc_stack.ss_sp = malloc(STACK_SIZE);
    newThread->context.uc_stack.ss_size = STACK_SIZE;
    newThread->context.uc_stack.ss_flags = NULL;

    if (newThread->context.uc_stack.ss_sp == NULL) {
        std::cout << "Error: Unable to allocate stack memory: " << 1024*64 << " bytes in the heap\n";
    }

    makecontext(&newThread->context, (void (*)()) &rpthread_start, 3, newThread, function, arg);
    map->put(threadCounter++, newThread);

    // Initialize the scheduler & main thread
    if (!isSchedCreated) {
        createSchedulerContext();
        createMainContext();

        run_queue[0].enqueue(map->get(MAIN_THREAD));
        run_queue[0].enqueue(newThread);

        isSchedCreated = true;

        // TODO Refactoring: Fix both Makefiles
/*        #ifndef MLFQ
            SCHED_TYPE = RR;
        #else
            SCHED_TYPE = MLFQ;
        #endif*/

        setupTimer();
    } else {
        run_queue[0].enqueue(newThread);
    }

    return 0;
};

void rpthread_start(tcb *currTCB, void (*function)(void *), void *arg) {
    function(arg);
    currTCB->status = FINISHED;
    free(currTCB->context.uc_stack.ss_sp);

    setcontext(&get_scheduler_tcb()->context);
}

/* give CPU possession to other user-level threads voluntarily */
int rpthread_yield() {
    tcb* currTCB = get_current_tcb();
    currTCB->status = READY;

    isYielding = true;

    swapcontext(&currTCB->context, &get_scheduler_tcb()->context);
    return 0;
};

/* terminate a thread */
void rpthread_exit(void *value_ptr) {
    tcb* currTCB = get_current_tcb();
    currTCB->status = FINISHED;
    currTCB->retVal = value_ptr;
    free(currTCB->context.uc_stack.ss_sp);

    if (currTCB->joiningThread != 0) {
        tcb* joinedTCB = map->get(currTCB->joiningThread);
        joinedTCB->status = READY;
        run_queue[0].enqueue(joinedTCB);
    }

    setcontext(&get_scheduler_tcb()->context);
};


/* Wait for thread termination */
int rpthread_join(rpthread_t thread, void **value_ptr) {
    tcb* currTCB = get_current_tcb();
    tcb* joinedTCB = map->get(thread);

    if (joinedTCB->status != FINISHED) {
        joinedTCB->joiningThread = currTCB->id;
        currTCB->status = BLOCKED;

        swapcontext(&currTCB->context, &get_scheduler_tcb()->context);
    }

    value_ptr = &joinedTCB->retVal;

    return 0;
};

/* initialize the mutex lock */
int rpthread_mutex_init(rpthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
    return 0;
};

/* aquire the mutex lock */
int rpthread_mutex_lock(rpthread_mutex_t *mutex) {
    bool prev = std::atomic_flag_test_and_set_explicit(&mutex->flag, std::memory_order_acquire);
    tcb* currTCB = get_current_tcb();

    if (prev) { // flag was previously true, so mutex is not acquired successfully
        mutex->queue.enqueue(currTCB->id);
        currTCB->status = BLOCKED;

        swapcontext(&currTCB->context, &get_scheduler_tcb()->context);
    }

    return 0;
};

/* release the mutex lock */
int rpthread_mutex_unlock(rpthread_mutex_t *mutex) {
    std::atomic_flag_clear_explicit(&mutex->flag, std::memory_order_release);

    for (int i = 0; i < mutex->queue.size(); i++) {
      int x = mutex->queue.get(i);
      map->get(x)->status = READY;
      run_queue[0].enqueue(map->get(x));
    }
    mutex->queue.clear();

    return 0;
};


/* destroy the mutex */
int rpthread_mutex_destroy(rpthread_mutex_t *mutex) {

    return 0;
};

/* scheduler */
static void schedule() {
    currentThread.tNum = SCHEDULER_THREAD;

    if(SCHED_TYPE == MLFQ) {
        sched_mlfq();
    } else if(SCHED_TYPE == RR) {
        sched_rr(0);
    } else {
        write(1, "!!!INVALID SCHEDULE TYPE!!!", 27);
    }
}

/* Round Robin (RR) scheduling algorithm */
static int sched_rr(int queueNum) {
    if (run_queue[queueNum].empty()) return 0;

    if(SCHED_TYPE == RR) {
        if (isThreadInactive(queueNum)) {
            run_queue[queueNum].dequeue();
        } else {
            run_queue[queueNum].enqueue(run_queue[queueNum].dequeue());
        }
    }

    tcb* currTCB = run_queue[queueNum].peek();
    currentThread = {currTCB->id, queueNum};
    setcontext(&currTCB->context);
}

/* Preemptive MLFQ scheduling algorithm */
static void sched_mlfq() {
    int qNum = currentThread.qNum;

    if (isThreadInactive(qNum)) {
        run_queue[qNum].dequeue();
    } else if(isYielding || isLastQueue(qNum)) {
        run_queue[qNum].enqueue(run_queue[qNum].dequeue());
    } else {
        run_queue[qNum + 1].enqueue(run_queue[qNum].dequeue());
    }
    isYielding = false;

    int currQueue = 0;
    while (currQueue < QUEUE_NUM) {
        sched_rr(currQueue);
        ++currQueue;
    }

}

void createSchedulerContext() {
    tcb* schedTCB = new tcb();
    schedTCB->id = 0;
    getcontext(&schedTCB->context);
    schedTCB->context.uc_link = NULL;
    schedTCB->context.uc_stack.ss_sp = malloc(STACK_SIZE);
    schedTCB->context.uc_stack.ss_size = STACK_SIZE;
    schedTCB->context.uc_stack.ss_flags = NULL;

    if (schedTCB->context.uc_stack.ss_sp == NULL) {
        std::cout << "Error: Unable to allocate stack memory: " << STACK_SIZE << " bytes in the heap\n";
    }

    makecontext(&schedTCB->context, (void (*)()) &schedule, 0);

    map->put(SCHEDULER_THREAD, schedTCB);
}

void createMainContext() {
    tcb* mainTCB = new tcb();
    mainTCB->id = 1;
    mainTCB->status = READY;

    map->put(MAIN_THREAD, mainTCB);
}

void setupTimer() {
    struct itimerval it_val;	/* for setting itimer */

    if (signal(SIGALRM, (void (*)(int)) timer_handler) == SIG_ERR) {
        perror("Unable to catch SIGALRM");
        exit(1);
    }
    it_val.it_value.tv_sec =     TIMESLICE/1000;
    it_val.it_value.tv_usec =    (TIMESLICE*1000) % 1000000;
    it_val.it_interval = it_val.it_value;
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }

    bool isTimerFiredOnce = false;
    while (!isTimerFiredOnce) {
        pause();
        isTimerFiredOnce = true;
    }
}

void timer_handler(int signum) {
/*    char* test = "DEBUG: Running setupTimer handler\n";
    write(1, test, strlen(test));*/

    // setupTimer expired, schedule next thread
    if (currentThread.tNum != SCHEDULER_THREAD) {
        swapcontext(&get_current_tcb()->context, &get_scheduler_tcb()->context);
    }
}

bool isLastQueue(int queueNum) {
    return queueNum >= QUEUE_NUM-1;
}

bool isThreadInactive(int queueNum) {
    return run_queue[queueNum].peek()->status == FINISHED || run_queue[queueNum].peek()->status == BLOCKED;
}