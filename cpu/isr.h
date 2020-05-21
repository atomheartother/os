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

// IRQ declarations
extern void irq00();
extern void irq01();
extern void irq02();
extern void irq03();
extern void irq04();
extern void irq05();
extern void irq06();
extern void irq07();
extern void irq08();
extern void irq09();
extern void irq0A();
extern void irq0B();
extern void irq0C();
extern void irq0D();
extern void irq0E();
extern void irq0F();

// ISR are 0-31
# define ISR_START 0x00
# define ISR_COUNT 0x20

// IRQ are 32-47
# define IRQ_START 0x20
# define IRQ_COUNT 0x10

#define PIC1 0x20 // IO base address for master PIC 
#define PIC2 0xA0 // IO base address for slave PIC 
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

typedef struct {
    u32 ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 intNumber, errCode;
    u32 eip, cs, eflags, useresp, ss;
}   interruptRegisters;

void isrInstall();
void isrHandler(const interruptRegisters r);

typedef void (*isrCallback)(const interruptRegisters);
void registerIsrCallback(u8 n, isrCallback handler);

#endif