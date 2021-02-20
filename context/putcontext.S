#include "defs.h"

    .text
    .global put_context
    .align 2
    .type put_context, @function
put_context:
    movl 4(%esp), %eax

    xorl %ecx, %ecx
    // put gs
    movl REG_OFFSET(REG_GS)(%eax), %ecx
    movw %cx, %gs
    // put ds
    movl REG_OFFSET(REG_FS)(%eax), %ecx
    movw %cx, %fs
    // put ds
    movl REG_OFFSET(REG_ES)(%eax), %ecx
    movw %cx, %es
    // put ds
    movl REG_OFFSET(REG_DS)(%eax), %ecx
    movw %cx, %ds

    // put eip and esp
    movl REG_OFFSET(REG_EIP)(%eax), %ecx
    movl REG_OFFSET(REG_ESP)(%eax), %esp
    pushl %ecx

    // put general registers, including ebp
    movl REG_OFFSET(REG_EBP)(%eax), %ebp
    movl REG_OFFSET(REG_EDI)(%eax), %edi
    movl REG_OFFSET(REG_ESI)(%eax), %esi
    movl REG_OFFSET(REG_EDX)(%eax), %edx
    movl REG_OFFSET(REG_ECX)(%eax), %ecx
    movl REG_OFFSET(REG_EBX)(%eax), %ebx
    movl REG_OFFSET(REG_EAX)(%eax), %eax

    ret
