#include "defs.h"

    .text
    .global switch_context
    .align 2
    .type switch_context, @function
switch_context:
    pushl %eax
    movl 0xc(%esp), %eax

    // get general registers
    movl %ebp, REG_OFFSET(REG_EBP)(%eax)
    movl %ebx, REG_OFFSET(REG_EBX)(%eax)
    movl %ecx, REG_OFFSET(REG_ECX)(%eax)
    movl %edx, REG_OFFSET(REG_EDX)(%eax)
    movl %esi, REG_OFFSET(REG_ESI)(%eax)
    movl %edi, REG_OFFSET(REG_EDI)(%eax)

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

