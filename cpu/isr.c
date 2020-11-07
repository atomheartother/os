#include "types.h"
#include "isr.h"
#include "idt.h"
#include "screen.h"
#include "ports.h"
#include "utils.h"

isrCallback isrCallbacks[256];

void isrInstall()
{
    // Register the 0-31 reserved Intel interrupts
    setIdtGate(0x00, (u32)isr00);
    setIdtGate(0x01, (u32)isr01);
    setIdtGate(0x02, (u32)isr02);
    setIdtGate(0x03, (u32)isr03);
    setIdtGate(0x04, (u32)isr04);
    setIdtGate(0x05, (u32)isr05);
    setIdtGate(0x06, (u32)isr06);
    setIdtGate(0x07, (u32)isr07);
    setIdtGate(0x08, (u32)isr08);
    setIdtGate(0x09, (u32)isr09);
    setIdtGate(0x0A, (u32)isr0A);
    setIdtGate(0x0B, (u32)isr0B);
    setIdtGate(0x0C, (u32)isr0C);
    setIdtGate(0x0D, (u32)isr0D);
    setIdtGate(0x0E, (u32)isr0E);
    setIdtGate(0x0F, (u32)isr0F);
    setIdtGate(0x10, (u32)isr10);
    setIdtGate(0x11, (u32)isr11);
    setIdtGate(0x12, (u32)isr12);
    setIdtGate(0x13, (u32)isr13);
    setIdtGate(0x14, (u32)isr14);
    setIdtGate(0x15, (u32)isr15);
    setIdtGate(0x16, (u32)isr16);
    setIdtGate(0x17, (u32)isr17);
    setIdtGate(0x18, (u32)isr18);
    setIdtGate(0x19, (u32)isr19);
    setIdtGate(0x1A, (u32)isr1A);
    setIdtGate(0x1B, (u32)isr1B);
    setIdtGate(0x1C, (u32)isr1C);
    setIdtGate(0x1D, (u32)isr1D);
    setIdtGate(0x1E, (u32)isr1E);
    setIdtGate(0x1F, (u32)isr1F);

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

    setIdtGate(IRQ0, (u32)irq00);
    setIdtGate(IRQ1, (u32)irq01);
    setIdtGate(IRQ2, (u32)irq02);
    setIdtGate(IRQ3, (u32)irq03);
    setIdtGate(IRQ4, (u32)irq04);
    setIdtGate(IRQ5, (u32)irq05);
    setIdtGate(IRQ6, (u32)irq06);
    setIdtGate(IRQ7, (u32)irq07);
    setIdtGate(IRQ8, (u32)irq08);
    setIdtGate(IRQ9, (u32)irq09);
    setIdtGate(IRQ10, (u32)irq0A);
    setIdtGate(IRQ11, (u32)irq0B);
    setIdtGate(IRQ12, (u32)irq0C);
    setIdtGate(IRQ13, (u32)irq0D);
    setIdtGate(IRQ14, (u32)irq0E);
    setIdtGate(IRQ15, (u32)irq0F);
    initIdt();
}

void isrHandler(const interruptRegisters r) {
    printMessage("Received interrupt: ");
    char code[4];
    int_to_ascii(r.intNumber, code);
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

void registerIsrCallback(u8 n, isrCallback handler) {
    isrCallbacks[n] = handler;
}