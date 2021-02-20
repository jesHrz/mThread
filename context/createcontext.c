#include "libcontext.h"
#include "defs.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>

extern void put_context(context_t *);

static void context_cleaner(context_t * ctlink)
{
    if (ctlink == NULL)
        exit(0);
    put_context(ctlink);
}

int create_context(context_t * context, func_t entry, 
        u32_t *stack, u32_t stack_size, 
        int argc, ...)
{
    int i;
    u32_t *sp, *args;
    va_list va;

    if (stack_size < argc + 2)
        return 1;

    sp = (u32_t*)(stack + stack_size - (argc + 2));
    context->gregs[REG_EIP] = (u32_t)entry;
    context->gregs[REG_ESP] = (u32_t)sp;

    args = sp;
    *args++ = (u32_t)context_cleaner;

    va_start(va, argc);
    for (i = 0; i < argc; i++) {
        *args++ = va_arg(va, u32_t);
    }
    va_end(va);

    *args = (u32_t)context->ctlink;

    return 0;
}
