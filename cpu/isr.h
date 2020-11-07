#ifndef CPU_ISR_H_
# define CPU_ISR_H_

#include "types.h"

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
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
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

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
// The number of ISRs as per https://en.wikipedia.org/wiki/Interrupt_descriptor_table#Hardware-generated_exceptions
# define ISR_COUNT 0x20

// IRQ are 32-47
# define IRQ_START 0x20
# define IRQ_COUNT 0x10

// IRQ values from: https://en.wikipedia.org/wiki/Interrupt_request_%28PC_architecture%29
// IRQ0 triggers the system timer
#define IRQ0 IRQ_START + 0
// IRQ1 is the keyboard interrupt handler
#define IRQ1 IRQ_START + 1
#define IRQ2 IRQ_START + 2
#define IRQ3 IRQ_START + 3
#define IRQ4 IRQ_START + 4
#define IRQ5 IRQ_START + 5
#define IRQ6 IRQ_START + 6
#define IRQ7 IRQ_START + 7
#define IRQ8 IRQ_START + 8
#define IRQ9 IRQ_START + 9
#define IRQ10 IRQ_START + 10
#define IRQ11 IRQ_START + 11
#define IRQ12 IRQ_START + 12
#define IRQ13 IRQ_START + 13
#define IRQ14 IRQ_START + 14
#define IRQ15 IRQ_START + 15

#define PIC1 0x20 // IO base address for master PIC 
#define PIC2 0xA0 // IO base address for slave PIC 
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

typedef struct {
    u32 ds;
    u32 edi, esi, ebp, uselessesp, ebx, edx, ecx, eax;
    u32 intNumber, errCode;
    u32 eip, cs, eflags, esp, ss;
}   interruptRegisters;

void isrInstall();
void isrHandler(const interruptRegisters r);

typedef void (*isrCallback)(const interruptRegisters);
void registerIsrCallback(u8 n, isrCallback handler);

#endif