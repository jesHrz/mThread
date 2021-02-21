#include "libmthread.h"
#include <stdio.h>

struct node {
    int a;
    char b;
};

void func1(int arg1)
{
    int i;
    printf("func1: arg=%d\n", arg1);
    for (i = 0; i < 10; ++i) {
        printf("func1: ---- %d ----\n", i);
    }
}

void func2(int *arg2)
{
    int i;
    printf("func2: arg=%p(%d)\n", arg2, *arg2);
    for (i = 0; i < 5; ++i) {
        printf("func2: ---- %d ----\n", i);
        mthread_yield();
    }
    mthread_exit(777);
}

unsigned int func3(struct node *arg3)
{
    printf("func3: node.a=%d node.b=%c\n", arg3->a, arg3->b);
    mthread_sleep(3000);
    return 0xdeadbeef;
}

int func4()
{
    int i, n, m;
    int ret = 0;
    for (i = 0; i < 50; ++i) {
        printf("func4: ---- %d ----\n", i);
        for (n = 1; n <= 5000; ++n) {
            for (m = 1; m <= 5000; ++m) {
                ret += n * m;
            }
        }
    } 
    printf("func4: ret=%d\n", ret);
    return ret;
}

int main()
{
    int i, tid1, tid2, tid3, tid4;
    struct node tmp = {666, 'Z'};

    mthread_nice(0, 1000);

    tid1 = mthread_create((func_t)func1, 1, 0xabcd);
    tid2 = mthread_create((func_t)func2, 1, &tid1);
    tid3 = mthread_create((func_t)func3, 1, &tmp);
    tid4 = mthread_create((func_t)func4, 0);

    mthread_nice(tid4, 1);

    for (i = 0; i < 5; ++i) {
        printf("main: ---- %d ----\n", i);
        mthread_sleep(100);
    }

    mthread_join(tid1);
    // printf("main: func2(arg=%p)=%ld\n", &tid1, mthread_join(tid2));
    // printf("main: func3()=%lx\n", mthread_join(tid3));
    // printf("main: func4()=%ld\n", mthread_join(tid4));

    printf("main done\n");
    return 0;
}