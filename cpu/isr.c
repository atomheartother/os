#include "types.h"
#include "isr.h"
#include "idt.h"
#include "screen.h"

void isrInstall()
{
    const u32 isrAddresses[ISR_COUNT] = {
        (u32)isr00,
        (u32)isr01,
        (u32)isr02,
        (u32)isr03,
        (u32)isr04,
        (u32)isr05,
        (u32)isr06,
        (u32)isr07,
        (u32)isr08,
        (u32)isr09,
        (u32)isr0A,
        (u32)isr0B,
        (u32)isr0C,
        (u32)isr0D,
        (u32)isr0E,
        (u32)isr0F,
        (u32)isr10,
        (u32)isr11,
        (u32)isr12,
        (u32)isr13,
        (u32)isr14,
        (u32)isr15,
        (u32)isr16,
        (u32)isr17,
        (u32)isr18,
        (u32)isr19,
        (u32)isr1A,
        (u32)isr1B,
        (u32)isr1C,
        (u32)isr1D,
        (u32)isr1E,
        (u32)isr1F
    };
    for (u16 i = 0; i < ISR_COUNT; i++)
    {
        setIdtGate(i, isrAddresses[i]);
    }
    initIdt();
}

void isrHandler(const isrRegisters r) {
    printMessage("Received interrupt");
}