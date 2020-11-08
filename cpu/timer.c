#include "timer.h"
#include "isr.h"
#include "ports.h"

uint32_t tick = 0;

static void timerCallback() {
    tick++;
}

void init_timer(uint32_t freq) {
    /* Install the function we just wrote as IRQ0 */
    registerIsrCallback(IRQ0, timerCallback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / freq;
    unsigned char low  = (unsigned char)(divisor & 0xFF);
    unsigned char high = (unsigned char)( (divisor >> 8) & 0xFF);
    /* Send the command */
    outb(0x43, 0x36); /* Command port */
    outb(0x40, low);
    outb(0x40, high);
}