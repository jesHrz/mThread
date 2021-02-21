#ifndef __THREAD_H_
#define __THREAD_H_

#include "libcontext.h"
#include <stdarg.h>

#define NR_MTHREAD          32
#define STACK_SZ            1024
#define TIME_SLICE_MSEC     10

#define MTHREAD_ZOMBIE      0
#define MTHREAD_RUNNING     1
#define MTHREAD_SLEEP       2
#define MTHREAD_READY       3

struct mthread_struct {
    int tid;      // thread id

    context_t ctx;  // thread context

    int argc;
    u32_t *args;
    func_t entry;   // entry function    

    int priority;      // thread priority
    int counter;

    int status;     // thread status

    unsigned int alarm;     // time to wake up

    u32_t return_code;  // entry function return code

    u32_t stack[STACK_SZ];
};

int mthread_create(func_t entry, int argc, ...);
void mthread_yield();
void mthread_exit();
void mthread_sleep(int msec);
u32_t mthread_join(int tid);
int mthread_nice(int tid, int newpriority);


#endif //__THREAD_H_
