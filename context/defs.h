#ifndef __DEFS_H_
#define __DEFS_H_

#define REG_SIZE 4
#define REG_OFFSET(__reg) ((__reg) * REG_SIZE)

#define REG_ESP 0
#define REG_EIP 1
#define REG_EBP 2
#define REG_EAX 3
#define REG_EBX 4
#define REG_ECX 5
#define REG_EDX 6
#define REG_ESI 7
#define REG_EDI 8
#define REG_DS  9
#define REG_ES  10
#define REG_FS  11
#define REG_GS  12

#endif // __DEFS_H_
