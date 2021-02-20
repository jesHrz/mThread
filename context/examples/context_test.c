#include "libcontext.h"
#include <stdio.h>

int foo = 1;

void fun1(context_t *child, context_t *fa)
{
    int i;
    foo = 2;
    printf("fun1: child=%p fa=%p\n", child, fa);
    for (i = 0; i < 10; ++i) {
        printf("%d\n", i);
        if (i == 5) {
            switch_context(fa, child);
        }
    }
    printf("done\n");
}

int main()
{
    context_t child, fa;
    unsigned int stack[1024];
    get_context(&fa);
    get_context(&child);
    child.ctlink = &fa;
    create_context(&child, (void(*)(void))fun1, stack, 1024, 2, &child, &fa);
    printf("create context: child=%p fa=%p\n", &child, &fa);
    printf("before switch: foo=%d\n", foo);
    printf("switch to child\n");
    switch_context(&child, &fa);
    printf("return to main\n");
    printf("after switch: foo=%d\n", foo);
    
    printf("switch to child again\n");
    switch_context(&child, &fa);
    printf("return to main again\n");
    printf("done main\n");
    return 0;
}
