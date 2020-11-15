#include <stddef.h>
#include "idt.h"
#include "screen.h"

idtEntry idt[IDT_ENTRIES];
idtRegister idtReg;

void setIdtGate(uint16_t n, size_t address) {
    idt[n].offsetL = address & 0xffff;
    idt[n].selector = GDT_CS_OFFSET;
    idt[n].zero = 0;
    /*
    * Flags bitfields:
    * P: 1, used interrupts
    * DPL: 00, these are kernel interrupts
    * S: 0, interrupt
    * Type: 1110, 32-bit interrupt gate
    */
    idt[n].flags = 0b10001110;
    idt[n].offsetH = (address >> 16) & 0xffff;
}

void initIdt() {
    idtReg.base = (size_t)&idt;
    idtReg.limit = sizeof(idtEntry) * IDT_ENTRIES - 1; // -1 because sizes start at 1
    asm volatile("lidtl (%0)" : : "r" (&idtReg));
}