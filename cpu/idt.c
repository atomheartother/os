#include "idt.h"

void fillIdtGate(idtEntry* idt, u32 address) {
    idt->offsetL = address & 0xffff;
    idt->selector = GDT_CS_OFFSET;
    idt->zero = 0;
    /*
    * Flags bitfields:
    * P: 1, used interrupts
    * DPL: 00, these are kernel interrupts
    * S: 0, interrupt
    * Type: 1110, 32-bit interrupt gate
    */
    idt->flags = 0b10001110;
    idt->offsetH = (address >> 16) & 0xffff;
}