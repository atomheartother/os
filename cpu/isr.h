#ifndef CPU_ISR_H_
# define CPU_ISR_H_

#include "types.h"

// https://en.wikipedia.org/wiki/Interrupt_descriptor_table#Hardware-generated_exceptions
extern void isr00();
extern void isr01();
extern void isr02();
extern void isr03();
extern void isr04();
extern void isr05();
extern void isr06();
extern void isr07();
extern void isr08();
extern void isr09();
extern void isr0A();
extern void isr0B();
extern void isr0C();
extern void isr0D();
extern void isr0E();
extern void isr0F();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr1A();
extern void isr1B();
extern void isr1C();
extern void isr1D();
extern void isr1E();
extern void isr1F();

# define ISR_COUNT 0x20

typedef struct {
    u32 ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 intNumber, errCode;
    u32 eip, cs, eflags, useresp, ss;
}   isrRegisters;

void isrInstall();
void isrHandler(const isrRegisters r);

#endif