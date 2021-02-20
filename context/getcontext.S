#include "defs.h"

    .text
    .global get_context
    .align 2
    .type get_context, @function
get_context:
    pushl %eax
    movl 8(%esp), %eax

    // get general registers
    movl %ebp, REG_OFFSET(REG_EBP)(%eax)
    movl %ebx, REG_OFFSET(REG_EBX)(%eax)
    movl %ecx, REG_OFFSET(REG_ECX)(%eax)
    movl %edx, REG_OFFSET(REG_EDX)(%eax)
    movl %esi, REG_OFFSET(REG_ESI)(%eax)
    movl %edi, REG_OFFSET(REG_EDI)(%eax)

    // get eax
    popl %ecx
    movl %ecx, REG_OFFSET(REG_EAX)(%eax)

    // get eip
    movl (%esp), %ecx
    movl %ecx, REG_OFFSET(REG_EIP)(%eax)

    // get esp
    leal 4(%esp), %ecx
    movl %ecx, REG_OFFSET(REG_ESP)(%eax)

    xorl %ecx, %ecx
    // get segment registers
    movw %ds, %cx
    movl %ecx, REG_OFFSET(REG_DS)(%eax)
    movw %es, %cx
    movl %ecx, REG_OFFSET(REG_ES)(%eax)
    movw %fs, %cx
    movl %ecx, REG_OFFSET(REG_FS)(%eax)
    movw %gs, %cx
    movl %ecx, REG_OFFSET(REG_GS)(%eax)

    ret
