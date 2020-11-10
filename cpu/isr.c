#include <stddef.h>
#include <stdint.h>
#include "isr.h"
#include "idt.h"
#include "screen.h"
#include "ports.h"
#include "kstring.h"
#include "timer.h"
#include "keyboard.h"

isrCallback isrCallbacks[256];

void isrInstall()
{
    // Register the 0-31 reserved Intel interrupts
    setIdtGate(0x00, (size_t)isr0);
    setIdtGate(0x01, (size_t)isr1);
    setIdtGate(0x02, (size_t)isr2);
    setIdtGate(0x03, (size_t)isr3);
    setIdtGate(0x04, (size_t)isr4);
    setIdtGate(0x05, (size_t)isr5);
    setIdtGate(0x06, (size_t)isr6);
    setIdtGate(0x07, (size_t)isr7);
    setIdtGate(0x08, (size_t)isr8);
    setIdtGate(0x09, (size_t)isr9);
    setIdtGate(0x0A, (size_t)isr10);
    setIdtGate(0x0B, (size_t)isr11);
    setIdtGate(0x0C, (size_t)isr12);
    setIdtGate(0x0D, (size_t)isr13);
    setIdtGate(0x0E, (size_t)isr14);
    setIdtGate(0x0F, (size_t)isr15);
    setIdtGate(0x10, (size_t)isr16);
    setIdtGate(0x11, (size_t)isr17);
    setIdtGate(0x12, (size_t)isr18);
    setIdtGate(0x13, (size_t)isr19);
    setIdtGate(0x14, (size_t)isr20);
    setIdtGate(0x15, (size_t)isr21);
    setIdtGate(0x16, (size_t)isr22);
    setIdtGate(0x17, (size_t)isr23);
    setIdtGate(0x18, (size_t)isr24);
    setIdtGate(0x19, (size_t)isr25);
    setIdtGate(0x1A, (size_t)isr26);
    setIdtGate(0x1B, (size_t)isr27);
    setIdtGate(0x1C, (size_t)isr28);
    setIdtGate(0x1D, (size_t)isr29);
    setIdtGate(0x1E, (size_t)isr30);
    setIdtGate(0x1F, (size_t)isr31);

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

    setIdtGate(IRQ0, (size_t)irq00);
    setIdtGate(IRQ1, (size_t)irq01);
    setIdtGate(IRQ2, (size_t)irq02);
    setIdtGate(IRQ3, (size_t)irq03);
    setIdtGate(IRQ4, (size_t)irq04);
    setIdtGate(IRQ5, (size_t)irq05);
    setIdtGate(IRQ6, (size_t)irq06);
    setIdtGate(IRQ7, (size_t)irq07);
    setIdtGate(IRQ8, (size_t)irq08);
    setIdtGate(IRQ9, (size_t)irq09);
    setIdtGate(IRQ10, (size_t)irq0A);
    setIdtGate(IRQ11, (size_t)irq0B);
    setIdtGate(IRQ12, (size_t)irq0C);
    setIdtGate(IRQ13, (size_t)irq0D);
    setIdtGate(IRQ14, (size_t)irq0E);
    setIdtGate(IRQ15, (size_t)irq0F);
    initIdt();
}

void isrHandler(interruptRegisters* r) {
    printMessage("Received interrupt: ");
    char code[4];
    int_to_ascii(r->intNumber, code);
    printMessage(code);
}

void irqHandler(interruptRegisters* r) {
    if (r->intNumber >= IRQ8) outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
    if (r->intNumber < IRQ_START ||  r->intNumber >= IRQ_START + IRQ_COUNT) {
        printMessage("Got out of bounds IRQ handler. This should literally never happen.");
        return;
    }
    if (isrCallbacks[r->intNumber] != 0) {
        isrCallbacks[r->intNumber](r);
    }
}

void registerIsrCallback(unsigned char n, isrCallback handler) {
    isrCallbacks[n] = handler;
}

void irqInstall() {
    __asm__ __volatile__("sti");
    init_timer(50);
    init_keyboard();
}