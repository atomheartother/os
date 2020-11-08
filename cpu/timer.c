#include "timer.h"
#include "isr.h"
#include "ports.h"

u32 tick = 0;

static void timerCallback() {
    tick++;
}

void init_timer(u32 freq) {
    /* Install the function we just wrote as IRQ0 */
    registerIsrCallback(IRQ0, timerCallback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);
    /* Send the command */
    outb(0x43, 0x36); /* Command port */
    outb(0x40, low);
    outb(0x40, high);
}