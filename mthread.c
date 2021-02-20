#include "libmthread.h"

#ifdef MTHREAD_DEBUG
#include <stdio.h>
#endif

#include <stdlib.h>
#include <sys/time.h>
#include <sys/signal.h>

static struct mthread_struct init_thread = {
    .status = MTHREAD_RUNNING,
    .priority = 20,
    .counter = 20
};

static struct mthread_struct *current = &init_thread;
static struct mthread_struct *tasks[NR_MTHREAD] = {&init_thread, };

static void mthread_entry();
static void mthread_release(struct mthread_struct *);
static void schedule();

#define FIRST_TASK  tasks[0]
#define LAST_TASK   tasks[NR_MTHREAD - 1]

#define CURRENT_TIME \
({ \
struct timeval tv; \
gettimeofday(&tv, NULL); \
tv.tv_sec * 1000 + tv.tv_usec / 1000; \
})


int mthread_create(func_t entry, int argc, ...)
{
    va_list ap;

    int i, tid = 1;
    while(tid < NR_MTHREAD && tasks[tid])
        tid++;
    // can not find a free task
    if (tid >= NR_MTHREAD)
        return -1;

    struct mthread_struct **p = &tasks[tid];
    *p = (struct mthread_struct *)malloc(sizeof(struct mthread_struct));

    (*p)->tid = tid;

    get_context(&(*p)->ctx);
    if (create_context(&((*p)->ctx), mthread_entry, (*p)->stack, STACK_SZ, 0))
        return -1; 

    (*p)->argc = argc;
    (*p)->args = (u32_t *)malloc(argc * sizeof(u32_t));
    (*p)->entry = entry;
    va_start(ap, argc);
    for (i = 0; i < argc; i++)
        (*p)->args[i] = va_arg(ap, u32_t);
    va_end(ap);

    (*p)->priority = 20;
    (*p)->counter = 20;

    (*p)->alarm = 0;
    (*p)->return_code = 0;

#ifdef MTHREAD_DEBUG
    printf("create thread %d: entry=%p argc=%d\n", (*p)->tid, (*p)->entry, (*p)->argc);
#endif

    (*p)->status = MTHREAD_READY;
    return tid;
}

u32_t mthread_join(int tid)
{
    if (tid < 1 || tid >= NR_MTHREAD)
        return -1;

    struct mthread_struct **p = &tasks[tid];

    if (!*p)
        return -1;
    
#ifdef MTHREAD_DEBUG
    printf("mthread_join: struct at %p, tid=%d\n", *p, (*p)->tid);
#endif

    while((*p)->status != MTHREAD_ZOMBIE)
        schedule();

    u32_t return_code = (*p)->return_code;

    mthread_release(*p);
    free(*p);

    return return_code;
}

void mthread_yield()
{
#ifdef MTHREAD_DEBUG
    printf("mthread_yield: struct at %p, tid=%d\n", current, current->tid);
#endif
    current->status = MTHREAD_READY;
    schedule();
}

void mthread_exit(u32_t return_code)
{
    // get return code from %EAX
    current->return_code = return_code;
    current->status = MTHREAD_ZOMBIE;

#ifdef MTHREAD_DEBUG
    printf("mthread_exit: struct at %p, tid=%d, return_code=%ld(0x%lx)\n", current, current->tid, current->return_code, current->return_code);
#endif

    schedule();
}

unsigned int mthread_sleep(int msec)
{
    unsigned int oldalarm = current->alarm;
    current->alarm = CURRENT_TIME + msec;
    current->status = MTHREAD_SLEEP;
#ifdef MTHREAD_DEBUG
    printf("mthread_sleep: struct at %p, tid=%d, sleep until %u\n", current, current->tid, current->alarm);
#endif
    schedule();
    return oldalarm;
}

static void mthread_entry()
{
    int i;
    u32_t return_code;
    // push arguments to stack
    for (i = current->argc - 1; i >= 0; --i)
        __asm__ __volatile__("pushl %0"::"r"(current->args[i]):);
#ifdef MTHREAD_DEBUG
    printf("mthread_entry: struct at %p, tid=%d\n", current, current->tid);
    __asm__ __volatile__(
        "leal 16(%%esp), %%esi \n\t"
        "movl %%esi, %%esp \n\t"
        :::);
#endif
    __asm__ __volatile__("call *%1":"=a"(return_code):"r"(current->entry):);
    mthread_exit(return_code);
    /* can not enter there!!! */
    for(;;);
}

static void mthread_showtasks()
{
    int i;
    for (i = 0; i < NR_MTHREAD; i++) {
        if (!tasks[i])
            continue;
        printf("thread %d: tid=%d\tpriority=%d\tcounter=%d\tstatus=%d\talarm=%u\n",
            i, tasks[i]->tid, tasks[i]->priority, tasks[i]->counter, tasks[i]->status, tasks[i]->alarm);
    }
}

static void mthread_release(struct mthread_struct *p)
{
    if (!p)
        return;
    if (p->tid < 1 || p->tid >= NR_MTHREAD)
        return;

    tasks[p->tid] = NULL;
    free(p->args);
}

static void schedule(void)
{
    int i, next, c;
    unsigned int jiffies;
    struct mthread_struct **p;
    jiffies = CURRENT_TIME;
    for (p = &LAST_TASK; p >= &FIRST_TASK; --p)
        if (*p) {
            if ((*p)->alarm && (*p)->alarm < jiffies && (*p)->status == MTHREAD_SLEEP) {
                (*p)->alarm = 0;
                (*p)->status = MTHREAD_READY;
            }
        }

    while(1) {
        c = 0, next = 0;
        p = &tasks[i = NR_MTHREAD];
        jiffies = CURRENT_TIME;
        while (--i >= 0) {
            if (!*--p)
                continue;

            if ((*p)->alarm && (*p)->alarm < jiffies && (*p)->status == MTHREAD_SLEEP) {
                (*p)->alarm = 0;
                (*p)->status = MTHREAD_READY;
            }

            if ((*p)->status == MTHREAD_READY && (*p)->counter > c)
                c = (*p)->counter, next = i;
        }
        if (c)  break;
        for (p = &LAST_TASK; p >= &FIRST_TASK; p--)
            if (*p)
                (*p)->counter = ((*p)->counter >> 1) + (*p)->priority;
    }
#ifdef MTHREAD_DEBUG
    printf("schedule: next task %d status=%d\n", next, tasks[next]->status);
#endif
    
    if (tasks[next] != current) {
        struct mthread_struct *tmp = current;
        current = tasks[next];
        current->status = MTHREAD_RUNNING;
        switch_context(&current->ctx, &tmp->ctx);
    }
}

static void do_timer(int sig)
{
    if (sig == SIGALRM) {
        if ((--current->counter) > 0) return;
        current->counter = 0;
        current->status = MTHREAD_READY;
        schedule();
    }
}

__attribute__((constructor))
static int mthread_init(void)
{
    // set timer to signal SIGALRM
    struct itimerval timer;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000 * TIME_SLICE_MSEC;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000 * TIME_SLICE_MSEC;
    if (setitimer(ITIMER_REAL, &timer, NULL)) {
#ifdef MTHREAD_DEBUG
        perror("mthread: set timer failed");
#endif
        return 1;
    }
    signal(SIGALRM, do_timer);
    return 0;
}
