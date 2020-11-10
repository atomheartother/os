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
    setIdtGate(0x00, (uint32_t)isr0);
    setIdtGate(0x01, (uint32_t)isr1);
    setIdtGate(0x02, (uint32_t)isr2);
    setIdtGate(0x03, (uint32_t)isr3);
    setIdtGate(0x04, (uint32_t)isr4);
    setIdtGate(0x05, (uint32_t)isr5);
    setIdtGate(0x06, (uint32_t)isr6);
    setIdtGate(0x07, (uint32_t)isr7);
    setIdtGate(0x08, (uint32_t)isr8);
    setIdtGate(0x09, (uint32_t)isr9);
    setIdtGate(0x0A, (uint32_t)isr10);
    setIdtGate(0x0B, (uint32_t)isr11);
    setIdtGate(0x0C, (uint32_t)isr12);
    setIdtGate(0x0D, (uint32_t)isr13);
    setIdtGate(0x0E, (uint32_t)isr14);
    setIdtGate(0x0F, (uint32_t)isr15);
    setIdtGate(0x10, (uint32_t)isr16);
    setIdtGate(0x11, (uint32_t)isr17);
    setIdtGate(0x12, (uint32_t)isr18);
    setIdtGate(0x13, (uint32_t)isr19);
    setIdtGate(0x14, (uint32_t)isr20);
    setIdtGate(0x15, (uint32_t)isr21);
    setIdtGate(0x16, (uint32_t)isr22);
    setIdtGate(0x17, (uint32_t)isr23);
    setIdtGate(0x18, (uint32_t)isr24);
    setIdtGate(0x19, (uint32_t)isr25);
    setIdtGate(0x1A, (uint32_t)isr26);
    setIdtGate(0x1B, (uint32_t)isr27);
    setIdtGate(0x1C, (uint32_t)isr28);
    setIdtGate(0x1D, (uint32_t)isr29);
    setIdtGate(0x1E, (uint32_t)isr30);
    setIdtGate(0x1F, (uint32_t)isr31);

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

    setIdtGate(IRQ0, (uint32_t)irq00);
    setIdtGate(IRQ1, (uint32_t)irq01);
    setIdtGate(IRQ2, (uint32_t)irq02);
    setIdtGate(IRQ3, (uint32_t)irq03);
    setIdtGate(IRQ4, (uint32_t)irq04);
    setIdtGate(IRQ5, (uint32_t)irq05);
    setIdtGate(IRQ6, (uint32_t)irq06);
    setIdtGate(IRQ7, (uint32_t)irq07);
    setIdtGate(IRQ8, (uint32_t)irq08);
    setIdtGate(IRQ9, (uint32_t)irq09);
    setIdtGate(IRQ10, (uint32_t)irq0A);
    setIdtGate(IRQ11, (uint32_t)irq0B);
    setIdtGate(IRQ12, (uint32_t)irq0C);
    setIdtGate(IRQ13, (uint32_t)irq0D);
    setIdtGate(IRQ14, (uint32_t)irq0E);
    setIdtGate(IRQ15, (uint32_t)irq0F);
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