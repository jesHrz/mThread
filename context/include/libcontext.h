#ifndef __LIB_CONTEXT_H_
#define __LIB_CONTEXT_H_

#include "../defs.h"

#define u8_t    unsigned char
#define u16_t   unsigned short
#define u32_t   unsigned long
#define REG_NR   13
#define CTX_ESP(__ctx)  (__ctx).gregs[REG_EIP]
#define CTX_EAX(__ctx)  (__ctx).gregs[REG_EAX]

typedef void (*func_t)(void);

typedef u32_t regs_t[REG_NR];

typedef struct {
    regs_t gregs;
    struct context_t *ctlink;
} context_t;

int create_context(context_t *context, func_t entry, u32_t *stack, u32_t stack_size, int argc, ...);
void put_context(context_t *context);
void get_context(context_t *context);
void switch_context(context_t *new_context, context_t *old_context);

#endif // __LIB_CONTEXT_H_
