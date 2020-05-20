#include "types.h"
#include "isr.h"
#include "idt.h"
#include "screen.h"
#include "ports.h"

isrCallback isrCallbacks[ISR_COUNT];

void isrInstall()
{
    // Register the 0-31 reserved Intel interrupts
    const u32 isrAddresses[ISR_COUNT] = {
        (u32)isr00, (u32)isr01, (u32)isr02, (u32)isr03, (u32)isr04, (u32)isr05,
        (u32)isr06, (u32)isr07, (u32)isr08, (u32)isr09, (u32)isr0A, (u32)isr0B,
        (u32)isr0C, (u32)isr0D, (u32)isr0E, (u32)isr0F, (u32)isr10, (u32)isr11,
        (u32)isr12, (u32)isr13, (u32)isr14, (u32)isr15, (u32)isr16, (u32)isr17,
        (u32)isr18, (u32)isr19, (u32)isr1A, (u32)isr1B, (u32)isr1C, (u32)isr1D,
        (u32)isr1E, (u32)isr1F
    };
    for (u16 i = ISR_START; i < ISR_COUNT; i++)
    {
        setIdtGate(i, isrAddresses[i - ISR_START]);
    }

    // Notify the PIC we want to remap the IRQs to 32-47 (0x20 - 0x2F)
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);

    const u32 irqAddresses[IRQ_COUNT] = {
        (u32)irq00, (u32)irq01, (u32)irq02, (u32)irq03, (u32)irq04, (u32)irq05,
        (u32)irq06, (u32)irq07, (u32)irq08, (u32)irq09, (u32)irq0A, (u32)irq0B,
        (u32)irq0C, (u32)irq0D, (u32)irq0E, (u32)irq0F
    };
    for (u16 i = IRQ_START; i < IRQ_COUNT; i++)
    {
        setIdtGate(i, irqAddresses[i - IRQ_START]);
    }

    initIdt();
}

void isrHandler(const interruptRegisters r) {
    printMessage("Received interrupt: ");
    const char hexAlph[] = "0123456789ABCDEF";
    char code[3];
    const u8 lowerLetter = r.intNumber & 0x0F;
    const u8 higherLetter = (r.intNumber >> 4) & 0xF0;
    code[0] = hexAlph[higherLetter];
    code[1] = hexAlph[lowerLetter];
    code[2] = 0;
    printMessage(code);
}

void irqHandler(const interruptRegisters r) {
    if (r.intNumber >= 0x08) outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
    if (r.intNumber >= IRQ_COUNT) {
        printMessage("Got out of bounds IRQ handler. This should literally never happen.");
        return;
    }
    if (isrCallbacks[r.intNumber] != 0) {
        isrCallbacks[r.intNumber](r);
    }
}