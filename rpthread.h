// File:	rpthread.h

// List all group member's name: Marko Elez (me470), Paul John (pj242)
// username of iLab:
// iLab Server: cd.cs.rutgers.edu

#ifndef RTHREAD_T_H
#define RTHREAD_T_H

#define _GNU_SOURCE

/* To use Linux pthread Library in Benchmark, you have to comment the USE_RTHREAD macro */
#define USE_RTHREAD 1

#ifndef TIMESLICE
/* defined timeslice to 5 ms, feel free to change this while testing your code
 * it can be done directly in the Makefile*/
#define TIMESLICE 5
#endif

#define READY 0
#define SCHEDULED 1
#define BLOCKED_MUTEX 2
#define BLOCKED_JOIN 3
#define FINISHED 4

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <ucontext.h>
#include <csignal>
#include <vector>
#include <atomic>
#include "queue.h"

#define STACK_SIZE SIGSTKSZ


typedef uint rpthread_t;

typedef struct threadControlBlock {
    uint id;
    int status;
    ucontext_t context;
    void* retVal;
    uint joiningThread;
} tcb;

/* mutex struct definition */
typedef struct rpthread_mutex_t {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    Queue<uint> queue;
    uint owner = 0;
} rpthread_mutex_t;

typedef struct {
    uint tNum;
    int qNum;
} tTuple;


/* Function Declarations: */

/* create a new thread */
int rpthread_create(rpthread_t * thread, pthread_attr_t * attr, void
*(*function)(void*), void * arg);

/* start a new thread */
void rpthread_start(tcb *currTCB, void (*function)(void *), void *arg);

/* give CPU pocession to other user level threads voluntarily */
int rpthread_yield();

/* terminate a thread */
void rpthread_exit(void *value_ptr);

/* wait for thread termination */
int rpthread_join(rpthread_t thread, void **value_ptr);

/* initial the mutex lock */
int rpthread_mutex_init(rpthread_mutex_t *mutex, const pthread_mutexattr_t
*mutexattr);

/* aquire the mutex lock */
int rpthread_mutex_lock(rpthread_mutex_t *mutex);

/* release the mutex lock */
int rpthread_mutex_unlock(rpthread_mutex_t *mutex);

/* destroy the mutex */
int rpthread_mutex_destroy(rpthread_mutex_t *mutex);


#ifdef USE_RTHREAD
#define pthread_t rpthread_t
#define pthread_mutex_t rpthread_mutex_t
#define pthread_create rpthread_create
#define pthread_exit rpthread_exit
#define pthread_join rpthread_join
#define pthread_mutex_init rpthread_mutex_init
#define pthread_mutex_lock rpthread_mutex_lock
#define pthread_mutex_unlock rpthread_mutex_unlock
#define pthread_mutex_destroy rpthread_mutex_destroy
#endif

#endif
